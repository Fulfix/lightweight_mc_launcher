all:
	gcc mc_launcher.c -o mc_launcher -lssl -lcrypto
windows:
	gcc mc_launcher.c -o mc_launcher -lssl -lcrypto -D_WIN32
