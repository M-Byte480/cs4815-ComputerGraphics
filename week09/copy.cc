
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#if !defined(GLUT_WHEEL_UP)
# 	define GLUT_WHEEL_UP 3
#	define GLUT_WHEEL_DOWN 4
#endif


using namespace std;

float divide = 6.0f;

/*  Set initial size of the display window.  */
GLsizei winWidth = 600, winHeight = 600;

/*  Set size of world-coordinate clipping window.  */
GLfloat xwcMin = -50.0, xwcMax = 50.0;
GLfloat ywcMin = -50.0, ywcMax = 50.0;
GLfloat xTranslate = 0.0, yTranslate = 0.0;

bool ctrl = false;



class wcPt3D {
   public:
      GLfloat x, y, z;
};

wcPt3D ctrlPts [6] = 	{
			{-40.0, -40.0, 0.0},
			{-25.0, 60.0 , 0.0},
			{-10.0, 200.0, 0.0},
                 	{10.0, -200.0, 0.0},
			{25.0, -60.0 , 0.0},
			{40.0, 40.0, 0.0}
			};

	/*  Set example number of control points and number of
	 *  curve positions to be plotted along the Bezier curve.
     	 */
   GLint nCtrlPts = sizeof(ctrlPts) / sizeof(wcPt3D);
   GLint nBezCurvePts = 1000;


void setClippingWinCoords(wcPt3D * ctrlPts)
{

	// Index 0, is x and index 1 is y
	// lets assume the first is the the extreme for them all
	xwcMin = ctrlPts[0].x; xwcMax = ctrlPts[0].x;
	ywcMin = ctrlPts[0].y; ywcMax = ctrlPts[0].y;
	int incrementVarialbe = 5;
	for(int i = 0; i < nCtrlPts; i++)
	{
		if(ctrlPts[i].x < xwcMin) xwcMin = ctrlPts[i].x - incrementVarialbe;
		if(ctrlPts[i].x > xwcMax) xwcMax = ctrlPts[i].x + incrementVarialbe;

		if(ctrlPts[i].y < ywcMin) ywcMin = ctrlPts[i].y - incrementVarialbe;
		if(ctrlPts[i].y > ywcMax) ywcMax = ctrlPts[i].y + incrementVarialbe;
	}
	cout << "x min : " << xwcMin << "\nx max : " << xwcMax << endl;
	cout << "y min : " << ywcMin << "\ny max : " << ywcMax << endl;
}

void init (void)
{
   /*  Set color of display window to white.  */
   glClearColor (1.0, 1.0, 1.0, 0.0);
   setClippingWinCoords(ctrlPts);
}

void plotPoint (wcPt3D bezCurvePt)
{
    glBegin (GL_POINTS);
        glVertex2f (bezCurvePt.x, bezCurvePt.y);
    glEnd ( );
}

/*  Compute binomial coefficients C for given value of n.  */
void binomialCoeffs (GLint n, GLint * C)
{
   GLint k, j;

   for (k = 0;  k <= n;  k++) {
      /*  Compute n!/(k!(n - k)!).  */
      C [k] = 1;
      for (j = n;  j >= k + 1;  j--)
        C [k] *= j;
      for (j = n - k;  j >= 2;  j--)
        C [k] /= j;
   }
}

void computeBezPt (GLfloat t, wcPt3D * bezPt, GLint nCtrlPts,
                    wcPt3D * ctrlPts, GLint * C)
{
   GLint k, n = nCtrlPts - 1;
   GLfloat bezBlendFcn;

   bezPt->x = bezPt->y = bezPt->z = 0.0;

   /*  Compute blending functions and blend control points. */
   for (k = 0; k < nCtrlPts; k++) {
      bezBlendFcn = C [k] * pow (t, k) * pow (1 - t, n - k);
      bezPt->x += ctrlPts [k].x * bezBlendFcn;
      bezPt->y += ctrlPts [k].y * bezBlendFcn;
      bezPt->z += ctrlPts [k].z * bezBlendFcn;
   }
}

void bezier (wcPt3D * ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
   wcPt3D bezCurvePt;
   GLfloat t;
   GLint *C;

   /*  Allocate space for binomial coefficients  */
   C = new GLint [nCtrlPts];

   binomialCoeffs (nCtrlPts - 1, C);

   for (int i = 0;  i <= nBezCurvePts;  i++) {
      t = GLfloat (i) / GLfloat (nBezCurvePts);
      computeBezPt (t, &bezCurvePt, nCtrlPts, ctrlPts, C);
//      glBegin(GL_LINE_STRIP);
      	plotPoint (bezCurvePt);
//      glEnd ( );
   }
   delete [ ] C;
// glutSwapBuffers();
//	glBegin(GL_LINE_STRIP);
//
//	glEnd();
}

void displayFcn (void)
{
   glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.
//
   glPointSize (4);
   glColor3f (1.0, 0.0, 0.0);      //  Set point color to red.

//   setClippingWinCoords(ctrlPts);

   bezier (ctrlPts, nCtrlPts, nBezCurvePts);

   glColor3f(0.05, 0.0, 1.0);
   glBegin(GL_LINE_STRIP);
   for(int i = 0; i < nCtrlPts ; i ++ )	glVertex2f(ctrlPts[i].x, ctrlPts[i].y);
   glEnd();

   glColor3f(.2, 0.0, 0.5);
//   glBegin(GL_LINE_STRIP);
   for(int i = 0; i < nCtrlPts ; i ++ )	plotPoint(ctrlPts[i]);
//   glEnd();

   glutSwapBuffers();
}

wcPt3D scalePoint(float x, float y){
	float xDiff = xwcMax - xwcMin, yDiff = ywcMax - ywcMin;
	GLfloat newX = x / winWidth * xDiff - xwcMax - xTranslate;
	GLfloat newY = (winHeight - y) / winHeight * ywcMax - ywcMax - yTranslate;

	wcPt3D p;
	p.x = newX;
	p.y = newY;
	return p;
}

void keyboardEvent(unsigned char key, int x, int y){
	if(key == 'a'){
		ctrl = true;
	}

}

void keyboardUpEvent(unsigned char key, int x, int y){
	if(key == 'a'){
		ctrl = false;
	}
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
   /*  Maintain an aspect ratio of 1.0.  */
   GLint min = newHeight;
	if(newWidth < newHeight) min = newWidth;

   glViewport (0, 0, min, min);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ( );

   winWidth = newWidth;
   winHeight = newHeight;

   gluOrtho2D (xwcMin - xTranslate, xwcMax - xTranslate, ywcMin - yTranslate, ywcMax - yTranslate);

   glutPostRedisplay();
}

bool leftMouse = false;
int startX, startY;
wcPt3D clickedPoint;
bool graphClicked, point_clicked;

void mouseInput(int button, int state, int x, int y){
	clickedPoint;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		wcPt3D scaledPoint = scalePoint((float) x, (float) y);
		for(int i = 0; i < nCtrlPts; i++){
			if(fabs(ctrlPts[i].x - scaledPoint.x) < 10.0 && fabs(ctrlPts[i].y - scaledPoint.y) < 10.0){
				point_clicked = true;
				clickedPoint = ctrlPts[i];
			}
		}

		if(point_clicked){
			graphClicked = false;
		}else{
			graphClicked = true;
		}
	}else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
		cout << "Button Released " << endl;
	}
        else{
		leftMouse = true;
		startX = x;
		startY = y;
	}
	if((GLUT_ACTIVE_CTRL & glutGetModifiers()) > 0){
		if(button == GLUT_WHEEL_UP){
			float m = 1.05;
			xwcMin *= m;
			xwcMax *= m;
			ywcMin *= m;
			ywcMax *= m;
		}
		if(button == GLUT_WHEEL_DOWN){
			float m = 0.95;
			xwcMin *= m;
			xwcMax *= m;
			ywcMin *= m;
			ywcMax *= m;
		}
	}



	winReshapeFcn(winWidth, winHeight);
	displayFcn();
}

//wcPt3D selected;

void motionCallback(int x, int y){
	float translateX, translateY;

	if(leftMouse){
		bool point_hit = false;
		//int size = 10;
		//for(int i = 0; i < nCtrlPts ; i ++){
			//if(ctrlPts[i].x < x + size && ctrlPts[i].y < y + size
			//&& ctrlPts[i].x > x - size && ctrlPts[i].y > y - size){
				// selected = ctrlPts[i];
				//point_hit = true;
				//cout << "POINT CAUGHT : " << ctrlPts[i].x << " : " << ctrlPts[i].y << endl;
			//	break;
			//}
		//}

		if(!point_hit){
//			cout << "Point not hit" << endl;
		}
	}

	if( graphClicked ){
		translateX = (x - startX) / divide;
		translateY = (y - startY) / divide;
		startX = x;
		startY = y;
	}

	xwcMin += -translateX;
	xwcMax += -translateX;
	ywcMin += translateY;
	ywcMax += translateY;

	winReshapeFcn(winWidth, winHeight);
	displayFcn();
}

int main (int argc, char** argv)
{
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowPosition (50, 50);
   glutInitWindowSize (winWidth, winHeight);
   glutCreateWindow ("Bezier Curve");

   init ( );
   glutDisplayFunc (displayFcn);
   glutReshapeFunc (winReshapeFcn);
   glutMouseFunc (mouseInput);
   glutKeyboardFunc(keyboardEvent);
   glutKeyboardUpFunc(keyboardUpEvent);
   glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
//   glutMouseWheelFunc(mouseWheel);
   glutMotionFunc(motionCallback);
   glutMainLoop ( );
}
