#include <GL/glut.h>	// ==> gl.h and glu.h

void init (void)
{
  glClearColor(1.0, 1.0, 1.0, 0.0);

  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void lineSegmentGreen (void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_LINES);
  glVertex2i(0, 0);
  glVertex2i(150, 112);
  glEnd();

  glFlush();
}

int main (int argc, char ** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(50, 100);
  glutInitWindowSize(400, 300);
  glutCreateWindow("A Baby GL example");

  init();  // we will write this

  glutDisplayFunc(lineSegmentGreen); // and this
  glutMainLoop();
}
