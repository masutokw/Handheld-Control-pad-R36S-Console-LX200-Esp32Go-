# Makefile for R36S stuff
CFLAGS= -O2
LIBS = -lm
PROG = planets
INCLUDES = -I. -I/usr/include
LDFLAGS = -lm -lsqlite3  -lSDL2 -lSDL2_ttf -lGLEW -lGL -lnova
DEFINES = -DTARGET=1

all: English Spanish

English: esp32go.c
	gcc esp32go.c kpad.c net.c query.c sdl2utils.c   $(DEFINES)   -g  $(LDFLAGS)    -o  /roms/esp32go/esp32go.exec

Spanish: esp32go.c
	gcc esp32go.c kpad.c net.c query.c sdl2utils.c   $(DEFINES)   -DLANG -g $(LDFLAGS)  -o  /roms/esp32go/esp32goESP.exec
	
clean:
	rm -rf *o $(PROG)
