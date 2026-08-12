struct response* first_req() {
  char message[] = "client_id=708e91b5-99f8-4a1d-80ec-e746cbb24771&scope=XboxLive.signin offline_access";
  struct request_options o = { .body = { .data = message, .size = sizeof(message) - 1 } };
  struct response* r = requests_post("https://login.microsoftonline.com/consumers/oauth2/v2.0/devicecode", &o);

  return r;
}
struct response *polling_req(char* dc) {
  int len = snprintf(NULL, 0, "grant_type=urn:ietf:params:oauth:grant-type:device_code&client_id=708e91b5-99f8-4a1d-80ec-e746cbb24771&device_code=%s", dc);
  if (len < 0){
    return NULL;
  }
  char *message = malloc(len + 1);
  if (message == NULL) { return NULL;}
  snprintf(message, len + 1, "grant_type=urn:ietf:params:oauth:grant-type:device_code&client_id=708e91b5-99f8-4a1d-80ec-e746cbb24771&device_code=%s", dc);

  struct request_options o = { .body = { .data = message, .size = len } };

  struct response* r = requests_post("https://login.microsoftonline.com/consumers/oauth2/v2.0/token", &o);
  free(message);


  return r;
}
struct response *rt_req(char* refresh_token) {
  int len = snprintf(NULL, 0, "client_id=708e91b5-99f8-4a1d-80ec-e746cbb24771&scope=XboxLive.signin offline_access&grant_type=refresh_token&refresh_token=%s", refresh_token);
  if (len < 0){
    return NULL;
  }
  char *message = malloc(len + 1);
  if (message == NULL) { return NULL;}
  snprintf(message, len + 1, "client_id=708e91b5-99f8-4a1d-80ec-e746cbb24771&scope=XboxLive.signin offline_access&grant_type=refresh_token&refresh_token=%s", refresh_token);

  struct request_options o = { .body = { .data = message, .size = len } };

  struct response* r = requests_post("https://login.microsoftonline.com/consumers/oauth2/v2.0/token", &o);
  free(message);

  return r;
}
struct response *get_xbl(char *access_token) {
  int len = snprintf(NULL, 0, "{\"Properties\": {\"AuthMethod\": \"RPS\",\"SiteName\": \"user.auth.xboxlive.com\",\"RpsTicket\": \"d=%s\"},\"RelyingParty\": \"http://auth.xboxlive.com\",\"TokenType\": \"JWT\"}", access_token);
  if (len < 0){
    return NULL;
  }
  char *message = malloc(len + 1);
  if (message == NULL) { return NULL;}
  snprintf(message, len + 1, "{\"Properties\": {\"AuthMethod\": \"RPS\",\"SiteName\": \"user.auth.xboxlive.com\",\"RpsTicket\": \"d=%s\"},\"RelyingParty\": \"http://auth.xboxlive.com\",\"TokenType\": \"JWT\"}", access_token);

  struct request_options o = { .body = { .data = message, .size = len } };
  header_add(&o.header, "Content-Type", "application/json");

  struct response* r = requests_post("https://user.auth.xboxlive.com/user/authenticate", &o);

  free(message);
  free_header(&o.header);
  return r;
}

struct response *get_xsts(char *xbl) {
  int len = snprintf(NULL, 0, "{\"Properties\": {    \"SandboxId\": \"RETAIL\",    \"UserTokens\": [\"%s\"]  },  \"RelyingParty\": \"rp://api.minecraftservices.com/\",  \"TokenType\": \"JWT\"}", xbl);
  if (len < 0){
    return NULL;
  }
  char *message = malloc(len + 1);
  if (message == NULL) { return NULL;}
  snprintf(message, len + 1, "{\"Properties\": {    \"SandboxId\": \"RETAIL\",    \"UserTokens\": [\"%s\"]  },  \"RelyingParty\": \"rp://api.minecraftservices.com/\",  \"TokenType\": \"JWT\"}", xbl);

  struct request_options o = { .body = { .data = message, .size = len } };
  header_add(&o.header, "Content-Type", "application/json");

  struct response* r = requests_post("https://xsts.auth.xboxlive.com/xsts/authorize", &o);

  free_header(&o.header);
  free(message);
  return r;
}
struct response *get_mc_token(char *xsts, char *userhash) {
  int len = snprintf(NULL, 0, "{\"identityToken\": \"XBL3.0 x=%s;%s\"}", userhash, xsts);
  if (len < 0){
    return NULL;
  }
  char *message = malloc(len + 1);
  if (message == NULL) { return NULL;}
  snprintf(message, len + 1, "{\"identityToken\": \"XBL3.0 x=%s;%s\"}", userhash, xsts);

  struct request_options o = { .body = { .data = message, .size = len } };
  header_add(&o.header, "Content-Type", "application/json");

  struct response* r = requests_post("https://api.minecraftservices.com/authentication/login_with_xbox", &o);

  free_header(&o.header);
  free(message);
  return r;
}
struct response *get_user_info(char* mc_token) {
  int len = snprintf(NULL, 0, "Bearer %s", mc_token);
  if (len < 0){
    return NULL;
  }
  char *message = malloc(len + 1);
  if (message == NULL) { return NULL;}
  snprintf(message, len + 1, "Bearer %s", mc_token);
  struct request_options o = {0};
  header_add(&o.header, "Authorization", message);

	struct response* r = requests_get("https://api.minecraftservices.com/minecraft/profile", &o);

  free_header(&o.header);
  return r;
}
