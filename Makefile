# Compiler Version
CC=gcc

# Debugging flag -g
DEBUG=-g

# Target
TARGET=shell

# Compile with all errors and warnings
CFLAGS=-c -Wall $(DEBUG)

all: $(TARGET)

$(TARGET): shell.o
	$(CC) shell.o -o $(TARGET)

shell.o: shell.c
	$(CC) $(CFLAGS) shell.c

clean:
	rm *.o *~ $(TARGET)

