
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#if !defined(GLUT_WHEEL_UP)
# 	define GLUT_WHEEL_UP 3
#	define GLUT_WHEEL_DOWN 4
#endif
#define RED 1
#define GREEN 2
#define BLUE 3

#define SMALL 4
#define MEDIUM 5
#define LARGE 6

#define QUIT -1
float red = 1.0, green = 0.0, blue = 0.0;
float size = 15;
using namespace std;

float divide = 6.0f;
bool circle = false, rectangle = false, line = true;
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

    float red, blue, green;

    float penSize;
};

class Line{
    wcPt3D first, second;
};
wcPt3D firstPoint, secondPoint;

wcPt3D* clickedPoint;
int clickCount = 0;
vector<wcPt3D> lines;
vector<wcPt3D> circles;
vector<wcPt3D> rectangles;

int startX, startY;
bool centerClicked, pointClicked;
wcPt3D scaledPoint;

wcPt3D ctrlPts [2] = {
        {-200.0,  -200.0, 0.0},
        { 200.0,   200.0, 0.0}
};

GLint nCtrlPts = sizeof(ctrlPts) / sizeof(wcPt3D);


void setClippingWindowCoords(){

    xwcMin = ctrlPts[0].x; xwcMax = ctrlPts[0].x;
    ywcMin = ctrlPts[0].y; ywcMax = ctrlPts[0].y;

    int incrementVar = 10;

    for(int i = 0 ; i < nCtrlPts ; i ++){
        if(ctrlPts[i].x <= xwcMin) xwcMin = ctrlPts[i].x - incrementVar;
        if(ctrlPts[i].x >= xwcMax) xwcMax = ctrlPts[i].x + incrementVar;

        if(ctrlPts[i].y <= ywcMin) ywcMin = ctrlPts[i].y - incrementVar;
        if(ctrlPts[i].y >= ywcMax) ywcMax = ctrlPts[i].y + incrementVar;
    }
    cout << xwcMin << " : " << xwcMax << endl;
    cout << ywcMin << " : " << ywcMin << endl;

}

void init (void)
{
    /*  Set color of display window to white.  */
    glClearColor (1.0, 1.0, 1.0, 0.0);
    setClippingWindowCoords();
}

void plotPoint (wcPt3D bezCurvePt)
{
    glBegin (GL_POINTS);
    glVertex2f (bezCurvePt.x, bezCurvePt.y);
    glEnd ( );
}
void plotPoint (GLfloat x, GLfloat y)
{
    wcPt3D t;
    t.x = x;
    t.y = y;
    plotPoint(t);
}


void drawLine(wcPt3D start, wcPt3D destination){
    glColor3f(start.red, start.green, start.blue);
    glLineWidth(start.penSize);
    glBegin(GL_LINES);
        glVertex2f(start.x, start.y);
        glVertex2f(destination.x, destination.y);
    glEnd();
    glFlush();
}

void drawRect(wcPt3D start, wcPt3D destination){
    glColor3f(start.red, start.green, start.blue);

    glPointSize(start.penSize);
    GLfloat differenceX, differenceY;
    differenceX = -(start.x - destination.x);
    differenceY = -(start.y - destination.y);

    int smoothnessPerSide = 100;

    glBegin(GL_POINTS);
    float diffXSection = float(differenceX) / float(smoothnessPerSide);
    float diffYSection = float(differenceY) / float(smoothnessPerSide);

    for (int i = 0; i < smoothnessPerSide; i++) {
        glVertex2f(start.x + i * diffXSection, start.y);
    }
    for (int i = 0; i < smoothnessPerSide; ++i) {
        glVertex2f(start.x + differenceX, start.y + i*diffYSection);
    }
    for (int i = 0; i < smoothnessPerSide; ++i) {
        glVertex2f(start.x + differenceX - i*diffXSection, start.y + differenceY);
    }
    for (int i = 0; i < smoothnessPerSide; ++i) {
        glVertex2f(start.x, start.y + differenceY - i * diffYSection);
    }

    glEnd();
    glFlush();

}
float pi = 3.14159;
void drawCirc(wcPt3D start, wcPt3D destination){
    glPointSize(start.penSize);
    glColor3f(start.red, start.green, start.blue);

    float x1, x2, y1, y2;
    x1 = start.x;
    x2 = destination.x;
    y1 = start.y;
    y2 = destination.y;
    float radius = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1-y2));
    int smoothness = 1000;
    glBegin(GL_POINTS);

    for (int i = 0; i < smoothness; ++i) {
        GLfloat plotX, plotY;
        GLfloat theta = pi * 2 * (float(i) / float(smoothness));
        plotX = radius * cos(theta);
        plotY = radius * sin(theta);

        // plot and offset
        glVertex2f(plotX + start.x, plotY+ start.y);
    }

    glEnd();
    glFlush();

}
void setDrawings(void){

        cout << lines.capacity() << endl;

        // If odd elements, which means we invoked the function mid a click
        if(circles.capacity() % 2 == 1) circles.pop_back();
        if(rectangles.capacity() % 2 == 1) rectangles.pop_back();
        if(lines.capacity() % 2 == 1) lines.pop_back();
        try {
            for (int i = 0; i < circles.capacity() / 2; i++) {
                drawCirc(circles.at(i * 2), circles.at(i * 2 + 1));
            }
            for (int i = 0; i < rectangles.capacity() / 2; i++) {
                drawRect(rectangles.at(i * 2), rectangles.at(i * 2 + 1));
            }
            for (int i = 0; i < lines.capacity() / 2; i++) {
                cout << i << endl;
                drawLine(lines.at(i * 2), lines.at(i * 2 + 1));
            }
        }catch (exception e){
            cout << e.what() << endl;
        }

}

void displayFcn (void)
{
    glClear (GL_COLOR_BUFFER_BIT);   //  Clear display window.

    size = 2;
    glPointSize (size);
    glColor3f (1.0, 0.0, 0.0);      //  Set point color to red.
    // Plot lines
    glColor3f(0.05, 0.0, 1.0);

    // Two axis
    glBegin(GL_LINES);
    glVertex2f(-1000, 0);
    glVertex2f(1000,0);
    glVertex2f(0, 1000);
    glVertex2f(0, -1000);
    glEnd();


    setDrawings();

//     if(clickCount >= 1){
//        if(line) drawLine(firstPoint, secondPoint);
//        else if(circle) drawCirc(firstPoint, secondPoint);
//        else if(rectangle) drawRect(firstPoint, secondPoint);
//        clickCount = 0;
//    }
    glutSwapBuffers();
}

void updateDisplay(void){

}

wcPt3D scalePoint(float x, float y){
    float xDiff = xwcMax - xwcMin;
    float yDiff = ywcMax - ywcMin;
    // Formula from lecture
    GLfloat newX = x / winWidth * xDiff + xwcMin;
    GLfloat newY = (- y) / winHeight * yDiff + ywcMax;

    wcPt3D p;
    p.x = newX;
    p.y = newY;
    return p;
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

    gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);


    glutPostRedisplay();
}




void mouseInput(int button, int state, int x, int y){
    startX = x;
    startY = y;
    centerClicked = false;
    pointClicked = false;
    clickedPoint = NULL;
    scaledPoint = scalePoint(x, y);

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

            // If the point is clicked within the circle radius
            if (fabs( 0 - scaledPoint.x) < 15.0 &&
                fabs( 0 - scaledPoint.y) < 15.0) {
                cout << "Center Clicked" << endl;
                centerClicked = true;
                pointClicked = false;
            }else{
                pointClicked = true;
            }

    }else if((GLUT_ACTIVE_CTRL & glutGetModifiers()) > 0){
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
        winReshapeFcn(winWidth, winHeight);
    }

    if(pointClicked){
        glPointSize (size);

        if(clickCount == 1){
            secondPoint = scaledPoint;

            secondPoint.red = red;
            secondPoint.blue = blue;
            secondPoint.green = green;
            secondPoint.penSize = size;

            if(line) lines.push_back(secondPoint);
            else if(circle) circles.push_back(secondPoint);
            else if(rectangle) rectangles.push_back(secondPoint);

            glutPostRedisplay();

            clickCount = 0;
        }else{
            firstPoint = scaledPoint;

            firstPoint.red = red;
            firstPoint.blue = blue;
            firstPoint.green = green;
            firstPoint.penSize = size;
            clickCount++;

            if(line) lines.push_back(firstPoint);
            else if(circle) circles.push_back(firstPoint);
            else if(rectangle) rectangles.push_back(firstPoint);
        }
    }
}

void motionCallback(int x, int y){
    float translateX, translateY;
    // Drag translate
    if(centerClicked){
        translateX = (x - startX) / divide;
        translateY = (y - startY) / divide;

        startX = x;
        startY = y;

        xwcMin += -translateX;
        xwcMax += -translateX;
        ywcMin += translateY;
        ywcMax += translateY;
    }
    winReshapeFcn(winWidth, winHeight);
}
void processMenuEvents(int option) {
    switch (option) {
        case RED : red = 1.0; green = 0.0; blue = 0.0; break;
        case GREEN : red = 0.0; green = 1.0; blue = 0.0; break;
        case BLUE : red = 0.0; green = 0.0; blue = 1.0; break;

        case SMALL : size = 5; break;
        case MEDIUM : size = 15; break;
        case LARGE : size = 50; break;

        case QUIT : exit(0);
    }
}

void createGLUTMenus(){
    int menu, penSize, colour;

    penSize = glutCreateMenu(processMenuEvents);
    glutAddMenuEntry("Small", SMALL);
    glutAddMenuEntry("Medium", MEDIUM);
    glutAddMenuEntry("Large", LARGE);

    colour = glutCreateMenu(processMenuEvents);
    glutAddMenuEntry("Red",RED);
    glutAddMenuEntry("Blue",BLUE);
    glutAddMenuEntry("Green",GREEN);

    menu = glutCreateMenu(processMenuEvents);
    glutAddSubMenu("Pen Size", penSize);
    glutAddSubMenu("Colour", colour);
    glutAddMenuEntry("Quit", QUIT);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void keyboardEvent(unsigned char key, int x, int y){
    if(key >= 65 && key <= 90){
        key += 32;
    }
    if     (key == 'c'){circle = true; line = false; rectangle = false ; clickCount = 0;}
    else if(key == 'l'){circle = false; line = true; rectangle = false ; clickCount = 0;}
    else if(key == 'r'){circle = false; line = false; rectangle = true ; clickCount = 0;}
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition (50, 50);
    glutInitWindowSize (winWidth, winHeight);
    glutCreateWindow ("yFig 21308128");
    init ( );

    glutDisplayFunc (displayFcn);
    glutReshapeFunc (winReshapeFcn);
    glutMouseFunc (mouseInput);
    glutMotionFunc(motionCallback);
    glutKeyboardFunc(keyboardEvent);

    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    createGLUTMenus();

    glutMainLoop();
}
