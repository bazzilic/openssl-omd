CC ?= cc
RM ?= rm -f

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CFLAGS += -L/opt/homebrew/opt/openssl@3/lib -I/opt/homebrew/opt/openssl@3/include
endif

.PHONY: all clean

all: ecdh-openssl

ecdh-openssl: ecdh-openssl.c
	$(CC) $(CFLAGS) -Wall -Ofast -o ecdh-openssl ecdh-openssl.c -lssl -lcrypto

ecdh-openssl-dbg: ecdh-openssl.c
	$(CC) $(CFLAGS) -Wall -g -o ecdh-openssl ecdh-openssl.c -lssl -lcrypto

clean:
	$(RM) ecdh-openssl
