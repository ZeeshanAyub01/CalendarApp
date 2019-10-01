CC = gcc
CFLAGS = -Wall -std=c11 -g -Iinclude
LDFLAGS = -L

all: list parser

list: LinkedListAPI.o
	$(CC) $(CFLAGS) -shared bin/LinkedListAPI.o -o bin/liblist.so

parser: stringHelpers.o CalendarHelpers.o CalendarParser.o LinkedListAPI.o
	$(CC) $(CFLAGS) -shared bin/LinkedListAPI.o bin/CalendarHelpers.o bin/CalendarParser.o bin/stringHelpers.o -o ../libcal.so

stringHelpers.o: src/stringHelpers.c include/stringHelpers.h
	$(CC) $(CFLAGS) -fpic -c src/stringHelpers.c -o ./bin/stringHelpers.o

CalendarHelpers.o: src/CalendarHelpers.c include/CalendarHelpers.h 
	$(CC) $(CFLAGS) -fpic -c src/CalendarHelpers.c -o ./bin/CalendarHelpers.o

CalendarParser.o: src/CalendarParser.c include/CalendarParser.h 
	$(CC) $(CFLAGS) -fpic -c src/CalendarParser.c -o ./bin/CalendarParser.o

LinkedListAPI.o: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) $(CFLAGS) -fpic -c src/LinkedListAPI.c -o bin/LinkedListAPI.o

clean:
	rm ./bin/*.o
	rm ./bin/*.so

