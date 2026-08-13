CC = gcc
CC_WIN = x86_64-w64-mingw32-gcc

all:
	$(CC) mc_launcher.c -o mc_launcher -lssl -lcrypto

windows:
	$(CC_WIN) mc_launcher.c -o mc_launcher.exe -lssl -lcrypto -lws2_32
