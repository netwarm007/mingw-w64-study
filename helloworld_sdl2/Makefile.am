ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = autogen.sh
bin_PROGRAMS=helloworld_sdl2
resdir=$(prefix)/res/Lession2
res_DATA=res/Lession2/background.bmp res/Lession2/image.bmp
helloworld_sdl2_SOURCES=helloworld_sdl2.cpp
helloworld_sdl2_LDADD=-lmingw32 -lSDL2main -lSDL2
helloworld_sdl2_LDFLAGS=-mwindows -mconsole
AM_CXXFLAGS=-g -I$(top_srcdir)/include -Wall -std=c++11

