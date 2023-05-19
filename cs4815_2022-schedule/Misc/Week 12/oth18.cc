/*
 * Copyright (C) 2006 Jacek Rosik <jacek.rosik@ul.ie>
 */


#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

#ifdef WIN32
#   include <windows.h>
#else
#   include <sys/time.h>
#endif

#ifdef __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif


/* View frustum.  */
#define VIEW_FOVY       45.0
#define VIEW_ZNEAR      0.25
#define VIEW_ZFAR       250

/* Object names.  */
#define SPHERE          0
#define TORUS1          1
#define TORUS2          2


/* Data used for animation of objects.  */
struct _anim {
    float angle;
    float speed;
    float axis[3];
} anim[] = {
    { 0.0,  20.0,    { 1.0, 0.0, 0.0 } },
    { 0.0,  30.0,    { 0.0, 1.0, 0.0 } },
    { 0.0,  30.0,    { 1.0, 0.0, 0.0 } }
};


/**
 * Return number of miliseconds passed since an arbitrary moment in time.
 */ 
static int
get_time(void)
{
#ifndef WIN32
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000 + time.tv_usec / 1000;
#else
	return GetTickCount();
#endif
}


/** Draw the scene.  */
static void
draw(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Camera transformation.  */
    glTranslatef(0.0, 0.0, -5.0);

    /* Sphere */
    glLoadName(SPHERE);
    glRotatef(anim[SPHERE].angle,
              anim[SPHERE].axis[0],
              anim[SPHERE].axis[1],
              anim[SPHERE].axis[2]);
    glColor3f(0.8, 0.0, 0.0);
    glutSolidSphere(1.0, 32, 24);

    /* Torus  1 */
    glLoadName(TORUS1);
    glRotatef(anim[TORUS1].angle,
              anim[TORUS1].axis[0],
              anim[TORUS1].axis[1],
              anim[TORUS1].axis[2]);
    glColor3f(0.0, 0.8, 0.0);
    glutSolidTorus(0.2, 1.3, 24, 48);
    
    /* Torus  2 */
    glLoadName(TORUS2);
    glRotatef(anim[TORUS2].angle,
              anim[TORUS2].axis[0],
              anim[TORUS2].axis[1],
              anim[TORUS2].axis[2]);
    glColor3f(0.0, 0.0, 0.8);
    glutSolidTorus(0.2, 1.8, 24, 64);
}

/** Process the selection buffer and display results.  */
static void
process_hits(int nhits, unsigned size, GLuint *buffer)
{
    /* Hit and hit distance.  */
    GLuint hit = (GLuint) -1;
    GLuint zvalue = (GLuint) -1;

    /* Process all hits and choose the closest one.  */
    unsigned offset = 0;
    while (nhits > 0 && offset < size) {
        GLuint depth = buffer[offset];
        GLuint zmin = buffer[offset + 1];

        /* Select this hit if it is closer than the previous one.  */
        if (zmin <= zvalue) {
            zvalue = zmin;
            hit = buffer[offset + 3];
        } 

        /* Move to the next hit.  */
        offset += depth + 3;
        --nhits;
    }

    /* Present results to the user.  */
    if (hit != -1) {
        switch (hit) {
            case SPHERE:
                std::cout << "SPHERE" << std::endl;
                break;
            case TORUS1:
                std::cout << "TORUS1" << std::endl;
                break;
            case TORUS2:
                std::cout << "TORUS2" << std::endl;
                break;
        }
    }
}


/** GLUT display callback.  */
static void
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
    glutSwapBuffers();
}

/** GLUT mouse callback.  */
static void
mouse(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        return;

    /* Set up the selection buffer.  */
    GLuint buffer[64 * 1024];
    glSelectBuffer(sizeof(buffer), buffer);

    /* Switch to selection mode and initialize name stack.  */
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName((GLuint) -1);

    /* Retrieve viewport and projection matrix.  */
    GLint viewport[4];
    GLfloat projection[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetFloatv(GL_PROJECTION_MATRIX, projection);

    /* Save projection matrix.  */
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    
    /* Specify pick matrix and then multiply by saved projection.  */
    glLoadIdentity();
    gluPickMatrix(x, viewport[3] - y, 3, 3, viewport);
    glMultMatrixf(projection);

    draw();

    /* Revert to previously saved projection matrix.  */
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    /* Return to render mode and process hits if any.  */
    int nhits = glRenderMode(GL_RENDER);
    if (nhits > 0 )
        process_hits(nhits, sizeof(buffer), buffer);
}

/* GLUT reshape callback.  */
static void
reshape(int width, int height)
{
    if (height == 0 || width ==0)
        return;

    /* Update viewport.  */
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* Specify projection matrix, our view frustum.  */
    float aspect = ((float) width) / ((float) height);
    gluPerspective(VIEW_FOVY, aspect, VIEW_ZNEAR, VIEW_ZFAR);
}

/* GLUT timer callback.  */
static void
timer(int value)
{
    int time = get_time();
    float dt = (time - value) * 0.001;
    for (unsigned i = 0; i < sizeof(anim) / sizeof(anim[0]); ++i)
        anim[i].angle += anim[i].speed * dt;

    glutPostRedisplay();
    glutTimerFunc(5, timer, time);
}


/** OpenGL state initialization.  */
static void
initialize(void)
{
    /* Enable back face culling.  */
    glEnable(GL_CULL_FACE);

    /* Enable Z buffer.  */
    glEnable(GL_DEPTH_TEST);

    /* Set up lighting.  */
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
}


/** Entry point.  */
int
main(int argc, char *argv[])
{
    /* Initialize GLUT library.  */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Create window with the specified parameters.  */
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(512, 512);
    glutCreateWindow("glLast");

    /* Initialize OpenGL state.  */
    initialize();

    /* Register GLUT callback functions.  */
    glutDisplayFunc(display);
    //glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutTimerFunc(5, timer, get_time());

    /* Event processing loop.  */
    glutMainLoop();

    return EXIT_SUCCESS;
}


/* vi:set tw=72 sw=4 ts=4 et: */
