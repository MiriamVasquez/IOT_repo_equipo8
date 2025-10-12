CC ?= gcc
OUTPUT = client.out
SOURCES = client.c com.c
LIBS = -pthread com.h 

all:
	$(CC) $(CFLAGS) $(LIBS) $(SOURCES) -o $(OUTPUT)
clean:
	rm -f $(OUTPUT) 
fresh: clean all
