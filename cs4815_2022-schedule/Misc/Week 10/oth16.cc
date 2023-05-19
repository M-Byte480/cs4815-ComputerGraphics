/*
 * Copyright (C) 2006 Jacek Rosik <jacek.rosik@ul.ie>
 */


#ifdef WIN32
#   include <windows.h>
#endif

#include <cstdlib>

#ifdef __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif


/*
 * Global parameters
 */

/* Camera distance from cube.  */
float dist = 2.0;

/* Camera yaw and pitch.  */
float cyaw = 0.0;
float cpitch = 0.0;

/* Object yaw and pitch.  */
float oyaw = 0.0;
float opitch = 0.0;

/* Active transformations  */
bool orotate = false;
bool crotate = false;
bool zoom = false;

/* Mouse click position  */
int pos[] = { 0, 0 };

/* Rotate and zoom coefficients  */
float rcoeff = 1.0;
float zcoeff = 0.01;

enum Mode {
    MODE_WIREFRAME,
    MODE_SOLID,
    MODE_MIXED
} mode = MODE_SOLID;

enum Object {
    OBJECT_CUBE,
    OBJECT_SPHERE,
    OBJECT_TORUS,
    OBJECT_TEAPOT
} object = OBJECT_TEAPOT;


struct Light {
    float ambient[4];
    float diffuse[4];
    float specular[4];
} light = {
    { 0.1, 0.1, 0.2, 1.0 },
    { 0.8, 0.8, 0.8, 1.0 },
    { 1.0, 1.0, 1.0, 1.0 }
};


struct Material {
    float emission[4];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float shininess[1];
} material = {
    { 0.0, 0.0, 0.0, 1.0 },
    { 0.8, 0.8, 0.8, 1.0 },
    { 0.8, 0.8, 0.8, 1.0 },
    { 1.0, 1.0, 1.0, 1.0 },
    { 96 }
};


void
draw_object(bool wireframe)
{
    if (!wireframe) {
        /* Apply material.  */
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material.emission);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);
        glEnable(GL_LIGHTING);
    } else {
        glDisable(GL_LIGHTING);
        glColor3f(0.0, 1.0, 0.0);
    }

    switch (object) {
        case OBJECT_CUBE:
            if (wireframe)
                glutWireCube(1.0);
            else
                glutSolidCube(1.0);
            break;
        case OBJECT_SPHERE:
            if (wireframe)
                glutWireSphere(0.5, 32, 24);
            else
                glutSolidSphere(0.5, 32, 24);
            break;
        case OBJECT_TORUS:
            if (wireframe)
                glutWireTorus(0.2, 0.5, 24, 32);
            else
                glutSolidTorus(0.2, 0.5, 24, 32);
            break;
       case OBJECT_TEAPOT:
       default:
            GLint front;
            glGetIntegerv(GL_FRONT_FACE, &front);
            glFrontFace(front == GL_CCW ? GL_CW : GL_CCW);

            if (wireframe)
                glutWireTeapot(0.5);
            else
                glutSolidTeapot(0.5);

            glFrontFace(front);
            break;
    }
}


void
draw(void)
{
    /* Draw object.  */
    switch (mode) {
        case MODE_WIREFRAME:
            draw_object(true);
            break;
        case MODE_SOLID:
            draw_object(false);
            break;
        case MODE_MIXED:
            draw_object(false);
            draw_object(true);
            break;
    }
}


bool
init(int argc, char *argv[])
{
    return true;
}


void
menu(int value)
{
    glutPostRedisplay();
}

void
menu_shading(int value)
{
    if (value)
        glShadeModel(GL_SMOOTH);
    else
        glShadeModel(GL_FLAT);
    glutPostRedisplay();
}

void
menu_mode(int value)
{
    mode = (Mode) value;
    glutPostRedisplay();
}

void
menu_object(int value)
{
    object = (Object) value;
    glutPostRedisplay();
}


void
create_menu(void)
{
    int shading = glutCreateMenu(menu_shading);
    glutAddMenuEntry("Flat", 0); 
    glutAddMenuEntry("Smooth", 1); 

    int mode = glutCreateMenu(menu_mode);
    glutAddMenuEntry("Wireframe", MODE_WIREFRAME); 
    glutAddMenuEntry("Solid", MODE_SOLID); 
    glutAddMenuEntry("Mixed", MODE_MIXED); 

    int object = glutCreateMenu(menu_object);
    glutAddMenuEntry("Cube", OBJECT_CUBE); 
    glutAddMenuEntry("Sphere", OBJECT_SPHERE); 
    glutAddMenuEntry("Torus", OBJECT_TORUS); 
    glutAddMenuEntry("Teapot", OBJECT_TEAPOT); 

    glutCreateMenu(menu);
    glutAddSubMenu("Shading", shading);
    glutAddSubMenu("Mode", mode);
    glutAddSubMenu("Object", object);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

bool
init_gl(void)
{
    /* Set clear (background) color to light blue  */
    glClearColor(0.5, 0.5, 1.0, 1.0);

    /* Enable culling of back facing polygons.  */
    glEnable(GL_CULL_FACE);

    /* Enable Z buffer.  */
    glEnable(GL_DEPTH_TEST);

    /* Set up lighting.  */
    glLightfv(GL_LIGHT0, GL_AMBIENT, light.ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light.diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light.ambient);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    /* Enable polygon offset for mixed mode.  */
    glPolygonOffset(5.0, 5.0);
    glEnable(GL_POLYGON_OFFSET_FILL);

    create_menu();
    return true;
}



void
display(void)
{
    /* Clear color and Z buffers.  */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Reset any rotations and translations.  */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Position camera.  */
    glTranslatef(0.0, 0.0, -dist);
    glRotatef(-cpitch, 1.0, 0.0, 0.0);
    glRotatef(cyaw, 0.0, 1.0, 0.0);

    /* Set light parameters.  */
    float position [] = {1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    /* Draw scene.  */
    glPushMatrix();
    glRotatef(-opitch, 1.0, 0.0, 0.0);
    glRotatef(oyaw, 0.0, 1.0, 0.0);
    draw();
    glPopMatrix();

    /* Present contents of the back buffer.  */
    glutSwapBuffers();
}

void
motion(int x, int y)
{
    /* Skip if none of the buttons is pressed.  */
    if (!(orotate or crotate or zoom))
        return;

    /* Calculate relative movement.  */
    int dx = x - pos[0];
    int dy = y - pos[1];
    pos[0] = x;
    pos[1] = y;

    /* Update camera yaw and pitch  */
    if (orotate) {
        oyaw += dx * rcoeff;
        opitch -= dy * rcoeff;
    }
/* Update camera yaw and pitch  */ if (crotate) { cyaw += dx * rcoeff; cpitch -= dy * rcoeff; } /* Update camera distance  */ if (zoom) {
        dist -= dy * zcoeff;
        if (dist < 0.0)
            dist = 0.0;
        if (dist > 100.0)
            dist = 100.0;
    }

    /* For redrawing of the window contents.  */
    glutPostRedisplay();
}

void
mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
        orotate = (state == GLUT_DOWN);
    else if (button == GLUT_RIGHT_BUTTON)
        crotate = (state == GLUT_DOWN);
    else if (button == GLUT_MIDDLE_BUTTON)
        zoom = (state == GLUT_DOWN);

    pos[0] = x;
    pos[1] = y;
}


void
reshape(int width, int height)
{
    /* Update OpenGL viewport.  */
    glViewport(0, 0, width, height);

    /* Set camera projection.  */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = float(width) / float(height);
    gluPerspective(60.0, aspect, 0.25, 200.0);

    glutPostRedisplay();
}


int
main(int argc, char *argv[])
{
    /* Initialize GLUT library.  */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Initialize OpenGL independent state.  */
    if (!init(argc, argv))
        return EXIT_FAILURE;

    /* Create window with the specified parameters.  */
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(512, 512);
    glutCreateWindow("glCubes");

    /* After the context is created we can initialize OpenGL state.  */
    if (!init_gl())
        return EXIT_FAILURE;

    /* Register GLUT callback functions.  */
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);

    /* Event processing loop.  */
    glutMainLoop();

    return EXIT_SUCCESS;
}


/* vi:set tw=72 sw=4 ts=4 et: */
