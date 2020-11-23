OBJS = fileutil-30221005.o actions.o parseArguments.o utilityFunctions.o
CC = gcc
CFLAGS = -Wall -c -g
LFLAGS = -Wall

fileutil: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o fileutil

fileutil-30221005.o: fileutil-30221005.c actions.h parseArguments.h utilityFunctions.h
	$(CC) $(CFLAGS) fileutil-30221005.c

actions.o: actions.c actions.h utilityFunctions.h
	$(CC) $(CFLAGS) actions.c

parseArguments.o: parseArguments.c parseArguments.h utilityFunctions.h
	$(CC) $(CFLAGS) parseArguments.c

utilityFunctions.o: utilityFunctions.c utilityFunctions.h
	$(CC) $(CFLAGS) utilityFunctions.c
