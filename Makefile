# Name: Jamie Clements
# RedID: 131174989
# Makefile for the countsubstrings program

# C compiler
CC=gcc
# -std=c11  C variant to use, e.g. C 2011
# -Wall    show the necessary warning files
# -g3      include information for symbolic debugger e.g. gdb 
CFLAGS=-std=c11 -Wall -g3 -c -fpermissive

# Object files
OBJS = tree.o countsubstrings.o 

# Program name
PROGRAM = countsubstrings

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
$(PROGRAM) : $(OBJS)
	$(CC) -o $(PROGRAM) $^

tree.o : tree.h tree.cpp
	$(CC) $(CFLAGS) tree.cpp
	
countsubstrings.o : tree.h countsubstrings.cpp
	$(CC) $(CFLAGS) countsubstrings.cpp
	
clean :
	rm -f *.o $(PROGRAM)
