#This is the makefile of EpollTest

.PHONY:all
all:server client
server:
	gcc epoll_server.c -o server
client:
	gcc epoll_client.c -o client
clean:
	rm -f server client
