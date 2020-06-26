CC 	   = gcc
CFLAGS = -Wall -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -pedantic -Werror -g #-DNDEBUG -O3
BUILD  = main
LDLIBS = `sdl2-config --cflags --libs` -IGL -lGL -lGLEW

.PHONY: all clean run 

all: $(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o
	/bin/rm -f out.txt
	cd graphics; make clean
	cd audio; make clean

main: graphics/init.o graphics/loadshader.o graphics/transform.o audio/audio.o  main.c 

graphics/init.o: graphics/init.h 

graphics/loadshader.o:  graphics/loadshader.h 

graphics/transform.o: graphics/transform.h

audio/audio.o: audio/audio.h

run:
	make;
	./main


