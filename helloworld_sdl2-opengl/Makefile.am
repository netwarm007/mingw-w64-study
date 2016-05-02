ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = autogen.sh
bin_PROGRAMS=helloworld_sdl2opengl
helloworld_sdl2opengl_SOURCES=helloworld_sdl2-opengl.cpp
helloworld_sdl2opengl_LDADD=-lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32
helloworld_sdl2opengl_LDFLAGS=-mwindows -mconsole
AM_CXXFLAGS=-I$(top_srcdir)/include -Wall -std=c++11

