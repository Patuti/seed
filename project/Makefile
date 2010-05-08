PROJECT=libSeed.a
include Objects.in
<<<<<<< HEAD
=======
#TARGET = ../bin/debug/sdl/$(PROJECT)
>>>>>>> upstream/master
TARGET = ../bin/$(PROJECT)

AR = ar
AR_PARAM = cru
AS = as
NM = nm
STRIP = strip
LD = ld
GCC = g++
<<<<<<< HEAD
CFLAGS = -Wall -fPIC -g `sdl-config --cflags` $(CDEFS) -I$(SEEDSDK)/seed/include
CDEFS = -D_SDL_ -DDEBUG
LFLAGS = `sdl-config --libs` -lSDL_image -lopenal -lGL -lseed -logg -lvorbis -lvorbisfile -loggz -loggplay -ltheora
=======
CFLAGS = -Wall -fPIC -g `sdl-config --cflags` $(CDEFS) -I/media/d/SDK/seed/include
CDEFS = -D_SDL_ -DDEBUG -DYMEM_DEBUG -DYMEM_PARANOID_CHECK
LFLAGS = `sdl-config --libs` -L/bin -lSDL_image -lopenal -lGL -lseed -logg -lvorbis -lvorbisfile -loggz -loggplay -ltheora
>>>>>>> upstream/master
CPPFLAGS = $(CFLAGS)
LIBS = 



all:	$(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) $(AR_PARAM) $(TARGET) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(TARGET) *~
