#define REQUESTS_IMPLEMENTATION 
#include "requests.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "mc_requests.h"

char *parse_json(char *str, int quotes){
  int j, i, len, idx;
  char *ret;

  j = 0; // j represent the number of quotes
  for (i = 0; j != quotes; i++){
    if (str[i] == '"'){
      j++;
    }
  }

  j = i + 1; // j represent the idx of the end quotes
  while(str[j] != '"')
    j++;
  len = j - i + 1;
  ret = malloc(len);
  if (ret == NULL) { return NULL;}

  idx = 0;
  for (int k = i;k<=j - 1;k++){
    ret[idx++] = str[k];
  }
  ret[len - 1] = '\0';

  return ret;
}
void print_json(char *str, int quotes){
  int i,j;
  j = 0; // j represent the number of quotes
  for (i = 0; j != quotes; i++){
    if (str[i] == '"'){
      j++;
    }
  }
  printf("go to https://microsoft.com/link and paste this code: ");
  while(str[i] != '"')
    printf("%c", str[i++]);
  printf("\n");


}
int write_rt(char **access_token){// you have to free access_token your self
  int ret = 0;
  struct response *r = NULL, *reslt_pol = NULL;
  char *devicecode = NULL, *token_type = NULL, *refresh_token = NULL;
  FILE *fd;

  r = first_req();
  if(!r) { ret = 1; goto cleanup;}
  devicecode = parse_json(r->body.data, 7);
  print_json(r->body.data, 3);
  if (!devicecode) { ret = 1; goto cleanup;}

  for (;;) {
    reslt_pol = polling_req(devicecode);
    if (!reslt_pol) { ret = 1; goto cleanup;}

    token_type = parse_json(reslt_pol->body.data, 3);
    if (!token_type) { ret = 1; goto cleanup;}

    if (strcmp(token_type, "Bearer") == 0){
      break;
    }
    
    free(token_type); token_type = NULL;
    free_response(reslt_pol); reslt_pol = NULL;

    sleep(5);
  }
  *access_token = parse_json(reslt_pol->body.data, 15);
  refresh_token = parse_json(reslt_pol->body.data, 19);
  if (!*access_token || !refresh_token) { ret = 1; goto cleanup;}
  fd = fopen("refresh_token.txt", "w");
  if (fd == NULL) { ret = 1; goto cleanup;}
  fprintf(fd, "%s", refresh_token);
  fclose(fd);

cleanup:
  free_response(r);
  free_response(reslt_pol);
  free(devicecode);
  free(token_type);
  free(refresh_token);
  return ret;
}
int main(void) {
  int ret = 0, len = 0;
  char *rt = NULL, *mcslop_token = NULL, *mc_token = NULL, *user_name = NULL, *uuid = NULL;
  char *userhash = NULL, *xbl = NULL, *xsts = NULL;
  struct response *r = NULL;
  FILE *fp;
  fp = fopen("refresh_token.txt", "r");
  if (fp == NULL){
    if (write_rt(&mcslop_token)) { ret = 1; goto cleanup;}
  }
  if(mcslop_token == NULL){
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rt = malloc(len + 1);
    if (rt == NULL){ fclose(fp); ret = 1; goto cleanup;}
    fseek(fp, 0, SEEK_SET);
    fgets(rt, len + 1, fp);
    fclose(fp);

    r = rt_req(rt);
    if (!r) { ret = 1; goto cleanup; }

    rt = parse_json(r->body.data, 19);
    mcslop_token = parse_json(r->body.data, 15);
    if (!rt || !mcslop_token) { ret = 1; goto cleanup;}
    free_response(r);

    fp = fopen("refresh_token.txt", "w");
    if (fp == NULL) { ret = 1; goto cleanup;}
    fprintf(fp, "%s", rt);
    fclose(fp);
  }

  r = get_xbl(mcslop_token);
  if (r == NULL) { ret = 1; goto cleanup;}
  xbl = parse_json(r->body.data, 11);
  userhash = parse_json(r->body.data, 19);
  if (!xbl || !userhash) { ret = 1; goto cleanup;}
  free_response(r);

  r = get_xsts(xbl);
  if (!r) { ret = 1; goto cleanup;}
  xsts = parse_json(r->body.data, 11);
  if (!xsts) { ret = 1; goto cleanup;}
  free_response(r);

  r = get_mc_token(xsts, userhash);
  if (!r) { ret = 1; goto cleanup;}
  mc_token = parse_json(r->body.data, 7);
  if (!mc_token) { mc_token = NULL;}
  free_response(r);

  r = get_user_info(mc_token);
  if (!r) { ret = 1;goto cleanup;}
  user_name = parse_json(r->body.data, 7);
  uuid = parse_json(r->body.data, 3);
  printf("token :%s\n", mc_token);
  printf("name :%s\n", user_name);
  printf("uuid :%s\n", uuid);

cleanup:
  free(mcslop_token); 
  free_response(r);
  free(user_name);
  free(mc_token);
  free(userhash);
  free(xsts);
  free(uuid);
  free(xbl);
  free(rt);
  return ret;
}
