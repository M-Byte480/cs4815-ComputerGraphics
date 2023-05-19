//
// OpenGL program to draw "HI" using line segments.
// Draws only 2D lines.

#include <GL/glut.h>

// display callback function
void display()
{
  glClear(GL_COLOR_BUFFER_BIT); // clear the window

  // set line width, in pixels 
  glLineWidth(4.0);

  // default view coordinates go from -1.0 to 1.0 in both X and Y.
  // That is, the lower left corner is (-1.0, -1.0) and 
  // the upper right corner is (1.0, 1.0). The X axis is horizontal
  // and the Y axis is vertical.

  // Draw H
  glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 1.0); // set color to white
    glVertex2f(-0.8, -0.6);
    glVertex2f(-0.8, 0.6);
    glVertex2f(-0.2, 0.6);
    glVertex2f(-0.2, -0.6);
    glVertex2f(-0.8, 0.0);
    glVertex2f(-0.2, 0.0);
  glEnd();

  // Draw I
  glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0); // set color to red
    glVertex2f(0.5, -0.6);
    glVertex2f(0.5, 0.6);
    glVertex2f(0.3, 0.6);
    glVertex2f(0.7, 0.6);
    glVertex2f(0.3, -0.6);
    glVertex2f(0.7, -0.6);
  glEnd();

  glFlush();
}

// input keyboard function to quit with q or Q.
void quit(unsigned char key, int x, int y)
{
  if (key == 'q' || key == 'Q')
    exit(0);
}

int main (int argc, char** argv)
{
  glutInit(&argc, argv);        // initialize glut
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); // RGB, single buffer mode
  glutInitWindowSize(400,400);  // set window width, height in pixels
  glutInitWindowPosition(0,0);  // x, y from top left corner of screen
  glutCreateWindow(argv[0]);    // create window, with program name in titlebar
  glutDisplayFunc(display);     // set display callback function
  glutKeyboardFunc(quit);       // set keyboard callback function
  glutMainLoop();               // hand over control to glut

  return 0;
}
