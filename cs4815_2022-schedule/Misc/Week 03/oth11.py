
# Copyright (C) 2006 Jacek Rosik <jacek.rosik@ul.ie>
#


import sys

from OpenGL.GL import *
from OpenGL.GLUT import *


# GLUT display callback.
def draw_scene():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glRectf(-0.5, -0.5, 0.5, 0.5);

    glFlush()
    glutSwapBuffers()


if __name__ == '__main__':
    # Initialize GLUT and specify display mode.
    # We need double buffered RGBA color buffer and depth buffer.
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE)

    # Initialize window parameters and create it.
    glutInitWindowSize(800, 600)
    glutInitWindowPosition(50, 50)
    glutCreateWindow("CS4815: Tutorial 3 (pyOpenGL)")
    
    # Register GLUT callback functions.
    glutDisplayFunc(draw_scene)

    # Enter event processing loop.
    glutMainLoop()


# vi:set tw=72 sw=4 ts=4 et:
