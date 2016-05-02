ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = autogen.sh
bin_PROGRAMS=helloworld_sdl2
lession=Lession6
resdir=$(prefix)/res/$(lession)
res_DATA=res/$(lession)/sample.ttf
helloworld_sdl2_SOURCES=helloworld_sdl2.cpp
helloworld_sdl2_LDADD=-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
helloworld_sdl2_LDFLAGS=-mwindows -mconsole
AM_CXXFLAGS=-I$(top_srcdir)/include -Wall -std=c++11

