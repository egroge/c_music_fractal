CC 	   = gcc
CFLAGS = -Wall -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -pedantic -Werror -g #-DNDEBUG -O3
BUILD  = init.o loadshader.o

all: $(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o

audio.o: audio.h

