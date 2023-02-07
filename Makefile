CC ?= cc
RM ?= rm -f

.PHONY: all clean

all: ecdh-openssl

ecdh-openssl: ecdh-openssl.c
	$(CC) $(CFLAGS) -Wall -Ofast -o ecdh-openssl ecdh-openssl.c -lssl -lcrypto

ecdh-openssl-dbg: ecdh-openssl.c
	$(CC) $(CFLAGS) -Wall -g -o ecdh-openssl ecdh-openssl.c -lssl -lcrypto

clean:
	$(RM) ecdh-openssl
