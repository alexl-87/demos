.PHONY: all
all: server2

server: server.c
	gcc -O2 -pedantic -Wall -Werror -o $@ $< -lpthread

.PHONY: server2
server2: server2.c
	gcc -O2 -pedantic -Wall -Werror -o $@ $< -lpthread
	
.PHONY: clean
clean:
	rm -f server2

