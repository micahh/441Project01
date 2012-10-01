
# A simple makefile for 'myshell'

CC = gcc
CFLAGS = -Wall -g -std=gnu99



myshell : main.o parse.o executor.o joblist.o
	$(CC) -o $@ $^

main.o : main.c parse.h
	$(CC) $(CFLAGS) -o $@ -c $<
	
parse.o : parse.c parse.h joblist.h executor.h
	$(CC) $(CFLAGS) -o $@ -c $<
	
executor.o : executor.c executor.h joblist.h
	$(CC) $(CFLAGS) -o $@ -c $<

joblist.o : joblist.c joblist.h
	$(CC) $(CFLAGS) -o $@ -c $<


joblist_test: joblist_test.o joblist.o
	$(CC) -o $@ $^

joblist_test.o: joblist_test.c joblist.h
	$(CC) $(CFLAGS) -o $@ -c $<

	
clean : 
	$(RM) myshell *.o
	
	
