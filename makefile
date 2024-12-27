CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: build

build:
	gcc $(CFLAGS) main.c socket.c keyBoardThread.c screenThread.c senderThread.c receiverThread.c list.o -lpthread -o s-talk

run: build	
	./s-talk

valgrind: build
	valgrind --leak-check=full ./s-talk 2110 asb9838nu-e05 22111 

clean:
	rm -f s-talk