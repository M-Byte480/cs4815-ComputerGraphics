#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>

float m = 20;
/*  Set initial size of the display window.  */
GLsizei winWidth = 600, winHeight = 600;
GLfloat ymin = 0, ymax = 0, xmin = 0, xmax = 0;
/*  Set size of world-coordinate clipping window.  */
GLfloat xwcMin = -50.0, xwcMax = 50.0;
GLfloat ywcMin = -50.0, ywcMax = 50.0;
GLfloat vertices[][3] = {
        {0.0, 1, 0.0}, // top
        {-1, 0, 0.0}, // left
        {0.0, -2*1, 0.0}, // bottom
        {2*1, 0, 0.0} // right
};
GLfloat normals[][3] = {
        {0.0, 1, 0.0},
        {-1, 0, 0.0},
        {0.0, -2*1, 0.0},
        {2*1, 0, 0.0}
};
GLfloat colours[][3] = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 0.0}
};
void getBorders(){
    float extraDisplacement = 0;

    for (int i = 0; i < 4; ++i) {
        if(vertices[i][0] < xmin){
            xmin = vertices[i][0] - extraDisplacement;
        }else if(vertices[i][0] > xmax){
            xmax = vertices[i][0] + extraDisplacement;
        }

        if(vertices[i][1] < ymin){
            ymin = vertices[i][1] - extraDisplacement;
        }else if(vertices[i][1] > ymax){
            ymax = vertices[i][1] + extraDisplacement;
        }
    }

    for (int i = 0; i < 4; ++i) {
        if(normals[i][0] < xmin){
            xmin = normals[i][0] - extraDisplacement;
        }else if(normals[i][0] > xmax){
            xmax = normals[i][0] + extraDisplacement;
        }

        if(normals[i][1] < ymin){
            ymin = normals[i][1] - extraDisplacement;
        }else if(normals[i][1] > ymax){
            ymax = normals[i][1] + extraDisplacement;
        }
    }

}

void plotPoint (GLfloat x, GLfloat y, GLfloat* colour)
{
    glBegin (GL_POINTS);
    glColor3f(colour[0], colour[1], colour[2]);
    glVertex2f (x, y);
    glEnd ( );
}
float dotProduct(float *v1, float *v2){
    float result = 0;

    for (int i = 0; i < 3; i++) {
        result += result + v1[i] + v2[i];
    }
    return result;
}

void crossProduct(float *v1, float *v2, float res[]){
    res[0] = 0;
    res[1] = 0;
    res[2] = v1[0] * v2[1] - v2[1] * v1[0];
}

int pointInTriangleA(float x, float y){

    if(x >= -m || x <= 2*m)
        if(y >= x - 2*m &&  y>= -2*x-2*m && y <= 0){
            return 1;
        }
    return 0;

}
int pointInTriangleB(float x, float y){
if(x >= -m && x < 2*m )
    if(y <= x + m && y>=0 && y <= -.5 * x + m)
        return 1;

return 0;
}



int pointInShape(float x , float y ){

    if(pointInTriangleB(x,y ) || pointInTriangleA(x,y)) return 1;
    return 0;
}

void transcribe(float m) {
    vertices[0][1] =  m;    // 4
    vertices[1][0] = -m;    // 1
    vertices[2][1] = -2*m;  // 2
    vertices[3][0] = 2*m;   // 3


    for (int i = 0; i < 4; i++) {
        if(vertices[i][0] < xmin){
            xmin = vertices[i][0] ;
        }else if(vertices[i][0] > xmax){
            xmax = vertices[i][0] ;
        }

        if(vertices[i][1] < ymin){
            ymin = vertices[i][1] ;
        }else if(vertices[i][1] > ymax){
            ymax = vertices[i][1] ;
        }
    }

    float smoothness = 0.1;

    for (float y = ymin; y <= ymax; y += smoothness) {
        float ratio1, ratio2, ratio3, ratio4;
        float y1, y2, y3;

        y1 = vertices[1][1];
        y2 = vertices[2][1];

        GLfloat* intensity1 = colours[1];
        GLfloat* intensity2 = colours[2];

        if (y > 0) {
            y2 = vertices[0][1];
            intensity2 = colours[0];
        }
        ratio1 = (y - y2)  / (y1 - y2);
        ratio2 = (y1 - y)  / (y1 - y2);

        float intensity4[3] = {
                (ratio1 * intensity1[0]) + (ratio2 * intensity2[0]),
                (ratio1 * intensity1[1]) + (ratio2 * intensity2[1]),
                (ratio1 * intensity1[2]) + (ratio2 * intensity2[2]),
        };

        y3 = vertices[3][1];
        GLfloat* intensity3 = colours[3];
        ratio3 = (y - y2)  / (y3 - y2);
        ratio4 = (y3 - y)  / (y3 - y2);


        float intensity5[3] = {
                (ratio3 * intensity3[0]) + (ratio4 * intensity2[0]),
                (ratio3 * intensity3[1]) + (ratio4 * intensity2[1]),
                (ratio3 * intensity3[2]) + (ratio4 * intensity2[2]),
        };


        for (float x = xmin; x <= xmax; x += smoothness) {
            float x5, x4;

            x4 = xmin;
            x5 = xmax;

            GLfloat xratio1 = (x5 - x)  / (x5 - x4);
            GLfloat xratio2 = (x5 - x4)  / (x5 - x4);

            float interPolar[3] = {
                    (xratio1 * intensity4[0]) + (xratio2 * intensity5[0]),
                    (xratio1 * intensity4[1]) + (xratio2 * intensity5[1]),
                    (xratio1 * intensity4[2]) + (xratio2 * intensity5[2]),
            };

            if(pointInShape(x,y) != 0){
                plotPoint(x, y, interPolar);
            }

        }
    }
}


void displayFcn() {
    glClear(GL_COLOR_BUFFER_BIT);
    transcribe(m);
    glFlush();
    glutSwapBuffers();
}

void winReshapeFcn(int w, int h)
{
    float size = 40;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-size, size, -size * (GLfloat) h / (GLfloat) w,
                size * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-size * (GLfloat) w / (GLfloat) h,
                size * (GLfloat) w / (GLfloat) h, -size, size, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0);
    getBorders();
}

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("21308128's World");

    init();

    glutReshapeFunc(winReshapeFcn);
    glutDisplayFunc(displayFcn);
    glutMainLoop();
}