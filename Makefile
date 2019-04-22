#
#
SHELL := /bin/bash
#
#
ifeq ($(GITUSER),)
git-do:
	@echo "GITUSER undefined"

all: git-do

clean:
	@rm -f git-do *.o git_auth_token
else
# src/local.c
CC := gcc
SOURCES := token.o src/remote.c src/main.c
CFLAGS := -Wall -Wextra -pedantic -Wl,-O1 \
-mtune=generic -std=gnu11 -O3 -Ofast \
-fexpensive-optimizations -I./src \
-DGITUSER='"$(GITUSER)"'

token.o: git_auth_token
	@ . ./script/magic_process.sh

git-do: $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@ -lcurl -lssl
	@rm -f *.o

install: git-do
	@install -m0755 $^ /usr/lib/git-core/$^

clean:
	@rm -f git-do *.o git_auth_token

all: git-do

default: all

.PHONY: all install clean

#
endif