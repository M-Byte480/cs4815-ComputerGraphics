#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

class scrPt {
public:
    GLint x, y;
};

int displayType = 1;

const GLdouble twoPi = 6.283185;

void circleMidpoint (scrPt , GLint ); 	    // fn. defined in circle.cc
// ratio is 4:3
GLsizei winWidth = 400, winHeight = 300;    // Initial display window size.
GLint xRaster = 25, yRaster = 50; 	    // Initial raster position

GLubyte label[36] = { 'J', 'a', 'n',   'F', 'e', 'b',   'M', 'a', 'r',
		      'A', 'p', 'r',   'M', 'a', 'y',   'J', 'u', 'n',
		      'J', 'u', 'l',   'A', 'u', 'g',   'S', 'e', 'p',
		      'O', 'c', 't',   'N', 'o', 'v',   'D', 'e', 'c' };

GLint dataValue[12] = { 420, 342, 324, 310, 262, 185,
			190, 196, 217, 240, 213, 438 };

void init (void)
{
  glClearColor (1.0, 1.0, 1.0, 1.0);

  glMatrixMode (GL_PROJECTION);
  gluOrtho2D (0.0, (GLdouble)winWidth, 0.0, (GLdouble)winHeight);
  glViewport(0,0, winWidth, winHeight);
}

void barChart (void)
{
	GLint month, k;

	double width, height;
	yRaster = 50;
	// keep 4:3 aspect ratio
	width = winWidth * 0.001;
	height = 0.002 * winHeight;

	glColor3f (1.0, 0.0, 0.0);           //  Set bar color to red.

	for (k = 0; k < 12; k++){
		width = k * 50 * winWidth * 0.0015;
 		glRecti (20 + width, 75, width + 40, dataValue [k] * height);
	}

	glColor3f (0.0, 0.0, 0.0);          //  Set text color to black.
	xRaster = 20;                       //  Display chart labels.

	for (month = 0; month < 12; month++) {
		glRasterPos2i (xRaster, yRaster);
		for (k = 3*month; k < 3*month + 3; k++)
			glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, label [k]);
		xRaster += width / 11;
  	}
}

void lineChart (void)
{
	GLfloat x = 30;
	xRaster = 25; yRaster = 150;

	glClear(GL_COLOR_BUFFER_BIT); // clear display window

	glColor3f(0.0, 0.0, 1.0);     // set line colour to blue

	float w = winWidth * 0.0015;
	float h = winHeight * 0.002;
	// LINE
	glBegin(GL_LINE_STRIP);
	for (int k = 0; k < 12; k++)
	      glVertex2i(x + k*50 * w, dataValue[k]*h);
	glEnd();

	// STAR
	glColor3f(1.0, 0.0, 0.0);     // Set marker colour to red
	for (int k = 0; k < 12; k++)
	  {
		glRasterPos2i(25 + k*50 * w, (dataValue[k]-4) * h);
	  	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '*');
	  }

	glColor3f(0.0, 0.0, 0.0);     // black
	xRaster = 20;

	// LABES
	for (int m = 0; m < 12; m++)
	  {
		glRasterPos2i(20 + m*50 * w, yRaster * h);
	  	for (int k = 3*m; k < 3*m+3; k++)
	     		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, label[k]);
	  }
	glFlush();
}



void pieChart (void)
{
    scrPt circCtr, piePt;
    GLint radius = winWidth / 4;               // Circle radius.

    GLdouble sliceAngle, previousSliceAngle = 0.0;

    GLint k, nSlices = 12;                     // Number of Slices. 
    GLfloat dataValues[12] = {10.0, 7.0, 13.0, 5.0, 13.0, 14.0,
                               3.0, 16, 5.0, 3.0, 17.0, 8.0};
    GLfloat dataSum = 0.0;
 
    circCtr.x = winWidth / 2;                  // Circle center position.
    circCtr.y = winHeight / 2;
    circleMidpoint (circCtr, radius);  // Call midpoint circle-plot routine.

    for (k = 0; k < nSlices; k++)
        dataSum += dataValues[k];

    for (k = 0; k < nSlices; k++) {
        sliceAngle = twoPi * dataValues[k] / dataSum + previousSliceAngle;
        piePt.x = (GLint) (circCtr.x + radius * cos (sliceAngle));
        piePt.y = (GLint) (circCtr.y + radius * sin (sliceAngle));
        glBegin (GL_LINES);
            glVertex2i (circCtr.x, circCtr.y);
            glVertex2i (piePt.x, piePt.y);
        glEnd ( );
        previousSliceAngle = sliceAngle;
    }
}


#define BAR 1
#define PIE 2
#define LINE 3

#define ZOOM_OUT_ONE 4
#define ZOOM_OUT_HALF 5
#define ZOOM_IN_ONE 6
#define ZOOM_IN_DOUBLE 7

#define QUIT -1


void
 displayFcn (void)
{
	glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

	glColor3f (0.0, 0.0, 1.0);       //  Set circle color to blue.

	if (displayType == BAR){
		barChart( ) ;

	}else if(displayType == PIE){
		pieChart( );

	}else if(displayType == LINE){
		lineChart( );
	}

	glutSwapBuffers ( );
}

void winReshapeFcn (int newWidth, int newHeight)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ( );
    	gluOrtho2D (0.0, GLdouble (newWidth), 0.0, GLdouble (newHeight));

	/* Reset display-window size parameters.  */
	// 4:3 width to height

	winWidth = newWidth;
	winHeight = newHeight;

	glViewport(0, 0, winWidth, winHeight);

	glutPostRedisplay();
}

void processMenuEvents(int opt)
{
	switch(opt){
		case BAR :displayType = BAR; break;
		case PIE : displayType = PIE; break;
		case LINE : displayType = LINE; break;
		case QUIT : exit(0) ; break;

		case ZOOM_OUT_ONE: ; break;
		case ZOOM_IN_ONE: ; break;
		case ZOOM_OUT_HALF: ; break;
		case ZOOM_IN_DOUBLE: ; break;
	}
	displayFcn();
}

void keyboardInput(unsigned char k, int x, int y){
	if(k == 'Q' || k == 'q') exit(0);
	if(k == 'B' || k == 'b') displayType = BAR;
	if(k == 'C' || k == 'c') displayType = PIE;
	if(k == 'l' || k == 'L') displayType = LINE;
	displayFcn();
}


void createGlMenus(){
	int menu, displays, zoom;

	displays = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Bar chart",BAR);
	glutAddMenuEntry("Pie chart",PIE);
	glutAddMenuEntry("Line chart", LINE);


	zoom = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Zoom in -one", ZOOM_IN_ONE);
	glutAddMenuEntry("Zoom in Double", ZOOM_IN_DOUBLE);
	glutAddMenuEntry("Zoom out Half" , ZOOM_OUT_HALF);
	glutAddMenuEntry("Zoom out -one", ZOOM_OUT_ONE);

	menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Charts", displays);
	glutAddSubMenu("Zoom", zoom);
	glutAddMenuEntry("Quit",QUIT);
	glutAttachMenu(	GLUT_RIGHT_BUTTON);
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);
    //    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition (100, 100);
    glutInitWindowSize (winWidth, winHeight);
    glutCreateWindow ("Charts");

    init ( );
    glutDisplayFunc (displayFcn);
    glutReshapeFunc (winReshapeFcn);
    glutKeyboardFunc(keyboardInput);
    createGlMenus();
    glutMainLoop ( );
}
