#
# NAME: Lucas Chagas
# COURSE: CS230 / Spring, 2019
#

CFLAGS		:= -Wall -O1 -c
DEBUG		:= -gstabs
#DEBUG		:= -g
INCLUDES	:= -I ./

CC		:= gcc


all: jimmy_fallon

jimmy_fallon: jimmy_fallon.o 
	$(CC) $(DEBUG) jimmy_fallon.o -o jimmy_fallon -pthread

jimmy_fallon.o: jimmy_fallon.c 
	$(CC) $(DEBUG) $(CFLAGS) $(INCLUDES) jimmy_fallon.c -o jimmy_fallon.o

clean:
	rm -f *.o jimmy_fallon
