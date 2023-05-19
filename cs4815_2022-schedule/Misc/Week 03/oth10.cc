/*
 * Copyright (C) 2006 Jacek Rosik <jacek.rosik@ul.ie>
 */


#ifdef __WIN32__
#   include <windows.h>
#endif

#include <cstdlib>
#include <iostream>

#include <SDL.h>

#if defined(__APPLE__)
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#endif


static void
draw_scene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);

    glFlush();
    SDL_GL_SwapBuffers();
}


int
main(int argc, char *argv[])
{
    /* Initialize SDL subsystems, we need only VIDEO.  */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE)) {
        std::cerr << "failed to initialize SDL: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }


    /* Create window using SDL.  */
    SDL_WM_SetCaption("CS4815: Tutorial 3 (SDL)", NULL);
    if (!SDL_SetVideoMode(800, 600, 32, SDL_OPENGL | SDL_DOUBLEBUF)) {
        std::cerr << "failed to set video mode: "
                  << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

#ifdef __APPLE__
    /* The initial SDL_VIDEOEXPOSE event is not sent on Mac OS X.
       That is why we have call `draw_scene()' for the first time explicitly.  */
    draw_scene();
#endif

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        if (!SDL_WaitEvent(&event))
            break;

        switch (event.type) {
            case SDL_VIDEOEXPOSE:
                draw_scene();
                break;
            
            case SDL_QUIT:
                quit = true;
                break;
        }
    }

    /* SDL shutdown and cleanup. */
    SDL_Quit();
    return EXIT_SUCCESS;
}


/* vi:set tw=72 sw=4 ts=4 et: */
