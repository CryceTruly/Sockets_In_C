#Makefile for the Socket Lab
#Creates 2 executables: one for the server and the other for the client
CC = gcc
CFLAGS = -Wall -pedantic -std=c99
EXEC1 = socserv 
EXEC2 = socclient
OBJS = socserv.o socclient.o
SRCS = socserv.c socclient.c


all: $(EXEC1) $(EXEC2)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS)

debug: $(SRCS) 
	$(CC) $(CFLAGS) -g -ggdb -c $(SRCS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC1) $(OBJS)
	$(CC) $(CFLAGS) -g -ggdb -o $(EXEC2) $(OBJS)
clean:
	rm -f *~
	rm -f *#
	rm -f ./socserv
	rm -f ./socclient
	rm -f *.o
	rm -f *.gch
