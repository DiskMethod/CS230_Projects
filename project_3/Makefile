#
# NAME: Lucas Chagas
# COURSE: CS230 / Spring, 2019
#

CFLAGS		:= -Wall -O1 -c 
DEBUG		:= -gstabs
#DEBUG		:= -g
INCLUDES	:= -I ./

CC		:= gcc


all: wordfreak

wordfreak: mywordhist.o mystring.o main.o 
	$(CC) $(DEBUG) mywordhist.o mystring.o main.o -o wordfreak

mywordhist.o: mywordhist.c mywordhist.h 
	$(CC) $(DEBUG) $(CFLAGS) $(INCLUDES) mywordhist.c -o mywordhist.o

mystring.o: mystring.c mystring.h
	$(CC) $(DEBUG) $(CFLAGS) $(INCLUDES) mystring.c -o mystring.o

main.o: main.c mystring.h mywordhist.h
	$(CC) $(DEBUG) $(CFLAGS) $(INCLUDES) main.c -o main.o

clean:
	rm -f *.o wordfreak
