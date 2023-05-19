/*
 * Copyright (C) 2006 Jacek Rosik <jacek.rosik@ul.ie>
 */


#ifdef __WIN32__
#   include <windows.h>
#endif

#include <cstdlib>

#if defined(__APPLE__)
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif


/* GLUT display callback.  */
static void
draw_scene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRectf(-0.5, -0.5, 0.5, 0.5);

    glFlush();
    glutSwapBuffers();
}


int
main(int argc, char *argv[])
{
    /* Initialize GLUT and specify display mode.
       We need double buffered RGBA color buffer.  */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    /* Initialize window parameters and create it.  */
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("CS4815: Tutorial 3 (GLUT)");
    
    /* Register GLUT callback functions.  */
    glutDisplayFunc(draw_scene);

    /* Enter event processing loop.  */
    glutMainLoop();

    return EXIT_SUCCESS;
}


/* vi:set tw=72 sw=4 ts=4 et: */
