#include <iostream>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "cleanup.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}


void SceneInit(int  width,int  height) {
    float ratio = (float) width / (float) height;    
    // Our shading model--Gouraud (smooth).    
    glShadeModel(GL_SMOOTH);    
    // Set the clear color.    
    glClearColor(0, 0, 0, 0);    
    // Setup our viewport.    
    glViewport(0, 0, width, height);    
    //Change to the projection matrix and set our viewing volume.    
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();    
    gluPerspective(60.0, ratio, 1.0, 100.0); 
}

void SceneShow() {
    // Clear the color and depth buffers.    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    
    // We don't want to modify the projection matrix. */    
    glMatrixMode(GL_MODELVIEW);    
    glLoadIdentity();    
    // Move down the z-axis.    
    glTranslatef(0.0, 0.0, -5.0);    
    //Draw a square    
    glBegin(GL_QUADS);    
    glColor3f(1.0f,0.0f,0.0f);    
    glVertex3f(-1.0f, -1.0f, 1.0f);    
    glColor3f(0.0f,1.0f,0.0f);    
    glVertex3f(1.0f, -1.0f, 1.0f);    
    glColor3f(0.0f,0.0f,1.0f);    
    glVertex3f(1.0f,1.0f,1.0f);    
    glColor3f(1.0f,1.0f,0.0f);    
    glVertex3f(-1.0f,1.0f,1.0f);    
    glEnd();    
}  

void OnResize(int width, int height) {
    if ( height == 0 ) {   
        height = 1;    
    }    
    //Reset View    
    glViewport(0, 0, (GLint)width, (GLint)height);    
    //Choose the Matrix mode    
    glMatrixMode(GL_PROJECTION);    
    //reset projection    
    glLoadIdentity();    
    //set perspection    
    gluPerspective(45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0);    
    //choose Matrix mode    
    glMatrixMode(GL_MODELVIEW);    
    glLoadIdentity();    
}

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

    /*
     * Now, we want to setup our requested
     * window attributes for our OpenGL window.
     * We want *at least* 5 bits of red, green
     * and blue. We also want at least a 16-bit
     * depth buffer.
     *
     * The last thing we do is request a double
     * buffered window. '1' turns on double
     * buffering, '0' turns it off.
     *
     * Note that we do not use SDL_DOUBLEBUF in
     * the flags to SDL_SetVideoMode. That does
     * not affect the GL attribute state, only
     * the standard 2D blitting setup.
     */
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    SDL_Window *window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == nullptr){
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

	SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(1);

    /*
     * At this point, we should have a properly setup
     * double-buffered window for use with OpenGL.
     */
    SceneInit(WINDOW_WIDTH, WINDOW_HEIGHT);

	//Our event structure
	SDL_Event event;
	bool quit = false;
	while (!quit){
		while (SDL_PollEvent(&event)){
			switch(event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					int tmpX, tmpY;
					SDL_GetWindowSize(window, &tmpX, &tmpY);
					OnResize(tmpX, tmpY);
				}
			}
		}
		//Render the scene
		SceneShow();
		SDL_GL_SwapWindow(window);
	}

    cleanup(window);
	SDL_Quit();
	
	return 0;
}

