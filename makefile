
# A simple makefile for 'circ.c'

CC = gcc
CFLAGS = -Wall -g -std=gnu99



parsetest : parsetest.o parse.o
	$(CC) -o $@ $^
	
parsetest.o : parsetest.c parse.h
	$(CC) $(CFLAGS) -o $@ -c $<
	
parse.o : parse.c parse.h
	$(CC) $(CFLAGS) -o $@ -c $<
	
clean : 
	$(RM) parsetest *.o
	
	