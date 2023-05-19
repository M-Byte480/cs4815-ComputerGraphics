#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <float.h>

const float m = 1;
GLfloat ymin = 0, ymax = 0, xmin = 0, xmax = 0;

int points[] = {};

GLfloat vertices[][3] = {
        {0.0, 1, 0.0},
        {-1, 0, 0.0},
        {0.0, -2*1, 0.0},
        {2*1, 0, 0.0}
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

void setBorder(GLfloat **vertices){
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

}



void transcribe(float m){
    glBegin(GL_POINTS);
    vertices[0][1] =  m; // 4
    vertices[1][0] = -m; // 1
    vertices[2][1] = -2*m; // 2
    vertices[3][0] = 2*m; // 3

    float extraDisplacement = 3;



    float smoothness = 0.8;
    for (int y = ymin; y <= ymax; y+= smoothness) {
        float ratio1, ratio2, ratio3, ratio4;
        float y1, y2, y3;

        y1 = vertices[1][1];
        y2 = vertices[2][1];

        GLfloat* intensity1 = colours[1];
        GLfloat* intensity2 = colours[2];

        ratio1 = (y - y2)  / (y1 - y2);
        ratio2 = (y1 - y)  / (y1 - y2);

//        intensity = (ratio1 * intensity1) +  (ratio2 * intensity2);
        float intensity4[3] = {
                (ratio1 * intensity1[0]) +  (ratio2 * intensity2[0]),
                (ratio1 * intensity1[1]) +  (ratio2 * intensity2[1]),
                (ratio1 * intensity1[2]) +  (ratio2 * intensity2[2])
        };



        float intensity5[3] = {
                (ratio3 * intensity1[0]) +  (ratio4 * intensity2[0]),
                (ratio3 * intensity1[1]) +  (ratio4 * intensity2[1]),
                (ratio3 * intensity1[2]) +  (ratio4 * intensity2[2])
        };

        for (int x = xmin; x <= xmax; x+= smoothness) {
            float x5, x4;

            x4 = xmin;
            x5 = xmax;

            GLfloat xratio1 = (x5 - x)  / (x - x4);
            GLfloat xratio2 = (x5 - x4)  / (x5 - x4);

            float interPolar[3] = {
                    (xratio1 * intensity4[0]) +  (xratio2 * intensity5[0]),
                    (xratio1 * intensity4[1]) +  (xratio2 * intensity5[1]),
                    (xratio1 * intensity4[2]) +  (xratio2 * intensity5[2])
            };

            glColor3f(interPolar[0],interPolar[1],interPolar[2]);
            glVertex3f(x, y, 0);
        }

    }

}


void scanLines(void){

}

void fillLines(GLfloat a){

}


// *
void polygon(int* p){
    //Scan lines
    glBegin(GL_POLYGON);
    for(int k = 0; k < 4; k++){
        int a = *(p+k);
        for(int i = 0; i < sizeof(p)/sizeof(int) ; i++){
            glColor3fv(colours[a]);
//			glNormal3fv(normals[a]);
            glVertex3fv(vertices[a]);
        }
    }

    glEnd();
}
// */
void colourSquare(void)
{
    int points[] = {0, 1, 2, 3};
//	polygon(points);

}

void display(void)
{
/* display callback, clear frame buffer and z buffer,
   rotate cube and draw, swap buffers */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
//	glRotatef(theta[0], 1.0, 0.0, 0.0);
//	glRotatef(theta[1], 0.0, 1.0, 0.0);
//	glRotatef(theta[2], 0.0, 0.0, 1.0);
//    transcribe(1);

    colourSquare();
    glFlush();
    glutSwapBuffers();
}



void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
                2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
                2.0 * (GLfloat) w / (GLfloat) h,
                -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv){
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("21308128's World");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
//		glutIdleFunc();
//		glutMouseFunc(mouse);
//		glutEnable(GL_DEPTH_TEST);
    glutMainLoop();
}
