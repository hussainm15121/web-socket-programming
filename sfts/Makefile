##
^I# Name: Hussain Mehdi
^I# Purpose: Priority Non-Preemptive Scheduler (PNP)
## 

# Makefile variables
CC = gcc
EXEC = sfts
CFLAGS = -pedantic
OBJ = sfts.o
$(EXEC) : $(OBJ)
^I$(CC) $(OBJ) -o $(EXEC)

all:
^I$(CC) $(CFLAGS) sfts.c h1.h -o sfts
exec:
^I./sfts 10
clean:
^Irm -rf *.o
^Irm -rf $(OBJ)
^Irm -rf *.exe
