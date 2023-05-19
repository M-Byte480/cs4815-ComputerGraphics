#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <GL/glut.h>

#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4
#endif

class wcPt3D
{
public:
    GLfloat x, y, z;
    wcPt3D()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    };
    wcPt3D(GLfloat xx, GLfloat yy, GLfloat zz)
    {
        x = xx;
        y = yy;
        z = zz;
    };
};

/*  Set initial size of the display window.  */
GLsizei winWidth = 600, winHeight = 600;

/*  Set size of world-coordinate clipping window.  */
GLfloat xwcMin, xwcMax;
GLfloat ywcMin, ywcMax;
GLfloat xOffset, yOffset;
GLfloat zoomLevel;

int downX, downY;
bool lineClicked = false;
bool pointClicked = false;
bool pointAdded = false;
wcPt3D* clickedPoint = NULL;

GLint nCtrlPts = 6;
wcPt3D* ctrlPts;


void initPoints(void)
{
    ctrlPts = (wcPt3D*)malloc(nCtrlPts * sizeof(wcPt3D));
    ctrlPts[0] = wcPt3D(-40, -40, 0);
    ctrlPts[1] = wcPt3D(-10, 100, 0);
    ctrlPts[2] = wcPt3D(10, -100, 0);
    ctrlPts[3] = wcPt3D(100, -100, 0);
    ctrlPts[4] = wcPt3D(100, 100, 0);
    ctrlPts[5] = wcPt3D(40, 40, 0);
}

void addPoint(wcPt3D point)
{
    nCtrlPts++;
    ctrlPts = (wcPt3D*)realloc(ctrlPts, nCtrlPts * sizeof(wcPt3D));
    ctrlPts[nCtrlPts - 16] = point;
}

void removePoint(int index)
{
    if (nCtrlPts == 1)
        return;

    wcPt3D* temp = (wcPt3D*)malloc(nCtrlPts * sizeof(wcPt3D));

    memcpy(temp, ctrlPts, nCtrlPts * sizeof(wcPt3D));
    nCtrlPts--;
    ctrlPts = (wcPt3D*)realloc(ctrlPts, nCtrlPts * sizeof(wcPt3D));

    for (int i = index; i < nCtrlPts; i++)
    {
        ctrlPts[i] = temp[i + 1];
    }

    free(temp);
}

void initClippingWindow(void)
{
    GLfloat min = 0.0, max = 0.0;

    for (int i = 0; i < nCtrlPts; i++)
    {
        if (ctrlPts[i].x < min)
            min = ctrlPts[i].x - 10.0;

        if (ctrlPts[i].x > max)
            max = ctrlPts[i].x + 10.0;

        if (ctrlPts[i].y < min)
            min = ctrlPts[i].y - 10.0;

        if (ctrlPts[i].y > max)
            max = ctrlPts[i].y + 10.0;
    }

    xwcMin = min;
    xwcMax = max;
    ywcMin = min;
    ywcMax = max;
    xOffset = 0.0;
    yOffset = 0.0;
    zoomLevel = 1.0;
}

void init(void)
{
    /*  Set color of display window to white.  */
    glClearColor(1.0, 1.0, 1.0, 0.0);
    initPoints();
    initClippingWindow();
}

void plotPoint(wcPt3D bezCurvePt)
{
    glBegin(GL_POINTS);
    glVertex2f(bezCurvePt.x, bezCurvePt.y);
    glEnd();
}

/*  Compute binomial coefficients C for given value of n.  */
void binomialCoeffs(GLint n, GLint* C)
{
    GLint k, j;

    for (k = 0; k <= n; k++)
    {
        /*  Compute n!/(k!(n - k)!).  */
        C [k] = 1;

        for (j = n; j >= k + 1; j--)
            C[k] *= j;

        for (j = n - k; j >= 2; j--)
            C[k] /= j;
    }
}

void computeBezPt(GLfloat t, wcPt3D* bezPt, GLint nCtrlPts, wcPt3D* ctrlPts, GLint* C)
{
    GLint k, n = nCtrlPts - 1;
    GLfloat bezBlendFcn;

    bezPt->x = bezPt->y = bezPt->z = 0.0;

    /*  Compute blending functions and blend control points. */
    for (k = 0; k < nCtrlPts; k++)
    {
        bezBlendFcn = C[k] * pow(t, k) * pow(1 - t, n - k);
        bezPt->x += ctrlPts[k].x * bezBlendFcn;
        bezPt->y += ctrlPts[k].y * bezBlendFcn;
        bezPt->z += ctrlPts[k].z * bezBlendFcn;
    }
}

void bezier(wcPt3D* ctrlPts, GLint nCtrlPts, GLint nBezCurvePts)
{
    wcPt3D bezCurvePt;
    GLfloat t;
    GLint* C;

    /*  Allocate space for binomial coefficients  */
    C = new GLint[nCtrlPts];

    binomialCoeffs(nCtrlPts - 1, C);

    for (int i = 0; i <= nBezCurvePts; i++)
    {
        t = GLfloat(i) / GLfloat(nBezCurvePts);
        computeBezPt(t, &bezCurvePt, nCtrlPts, ctrlPts, C);
        plotPoint(bezCurvePt);
    }

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);

    for (int i = 0; i < nCtrlPts; i++)
    {
        glVertex2f(ctrlPts[i].x, ctrlPts[i].y);
    }

    glEnd();

    glColor3f(0.0, 0.0, 1.0);

    for (int i = 0; i < nCtrlPts; i++)
    {
        plotPoint(ctrlPts[i]);
    }

    delete[] C;
}

wcPt3D scale(int x, int y)
{
    wcPt3D point;

    point.x = ((float)x / winWidth * (xwcMax - xwcMin)) - xwcMax - xOffset;
    point.y = ((float)(winHeight - y) / winHeight * ywcMax * 2.0) - ywcMax - yOffset;

    return point;
}

void zoom(int id)
{
    if (id == 0)
    {
        xwcMin /= 1.2;
        xwcMax /= 1.2;
        ywcMin /= 1.2;
        ywcMax /= 1.2;
        zoomLevel /= 1.2;
    }
    else
    {
        xwcMin *= 1.2;
        xwcMax *= 1.2;
        ywcMin *= 1.2;
        ywcMax *= 1.2;
        zoomLevel *= 1.2;
    }
}

void displayFcn(void)
{
    glClear(GL_COLOR_BUFFER_BIT);   //  Clear display window.

    /*  Set example number of control points and number of
     *  curve positions to be plotted along the Bezier curve.
     */
    GLint nBezCurvePts = 1000;

    glPointSize(4);
    glColor3f(1.0, 0.0, 0.0);      //  Set point color to red.

    bezier(ctrlPts, nCtrlPts, nBezCurvePts);
    glutSwapBuffers();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
    /*  Maintain an aspect ratio of 1.0.  */
    glViewport(0, 0, newHeight, newHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(xwcMin - xOffset, xwcMax - xOffset, ywcMin - yOffset, ywcMax - yOffset);

    glutPostRedisplay();
}

void mouseFcn(int button, int state, int x, int y)
{
    downX = x;
    downY = y;
    lineClicked = false;
    pointClicked = false;
    pointAdded = false;
    clickedPoint = NULL;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        wcPt3D scaledPoint = scale(x, y);

        for (int i = 0; i < nCtrlPts; i++)
        {
            if (fabs(ctrlPts[i].x - scaledPoint.x) < 10.0 && fabs(ctrlPts[i].y - scaledPoint.y) < 10.0)
            {
                pointClicked = true;
                clickedPoint = ctrlPts + i;
            }
        }

        lineClicked = pointClicked ? false : true;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        wcPt3D scaledPoint = scale(x, y);

        for (int i = 0; i < nCtrlPts; i++)
        {
            if (fabs(ctrlPts[i].x - scaledPoint.x) < 4.0 && fabs(ctrlPts[i].y - scaledPoint.y) < 4.0)
            {
                    pointAdded = true;
                removePoint(i);
            }
        }

        if (pointAdded)
            pointAdded = false;
        else
            addPoint(scaledPoint);
    }
    else if ((GLUT_ACTIVE_CTRL & glutGetModifiers()) > 0)
    {
        if (button == GLUT_WHEEL_DOWN)
            zoom(1);
        else if (button == GLUT_WHEEL_UP)
            zoom(0);
    }

    winReshapeFcn(winWidth, winHeight); // cannot use glutPostRedisplay() - gluOrtho2D needs to be computed with the new values
}

void dragFcn(int x, int y)
{
    if (lineClicked)
    {
        xOffset += (x - downX) * zoomLevel / 2.0;
        yOffset += (downY - y) * zoomLevel / 2.0;
        downX = x;
        downY = y;
    }
    else if (pointClicked && clickedPoint != NULL)
    {
        *clickedPoint = scale(x, y);
    }

    winReshapeFcn(winWidth, winHeight); // cannot use glutPostRedisplay() - gluOrtho2D needs to be computed with the new values
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Bezier Curve");

    init();
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(winReshapeFcn);
    glutMouseFunc(mouseFcn);
    glutMotionFunc(dragFcn);

    glutMainLoop();
}
