#include <stdlib.h>
#include <GL/glut.h>
#include <stdio.h>
#include <float.h>


#include <stdlib.h>
#include <math.h>
#include <stdio.h>



struct point2f{
    GLfloat x;
    GLfloat y;
    GLfloat r;
    GLfloat g;
    GLfloat b;
};


#define M 100
#define N 80

typedef struct point2f point2f;
GLfloat winWidth = 500, winHeight = 500;

const point2f vertexPts[4]={{0,M,1.0,0,0},{-M,0,0,1.0,0},{0,-2*M,0,0,1.0},{2*M,N,0,0,0}};
point2f pts_1[M+1],pts_2[2*M+1],pts_3[2*M+1+N],pts_4[2*M+1];

void createPoint(point2f);
void scanline(point2f,point2f);
void drawLines(point2f,point2f,point2f*);
void paintLine(point2f[],point2f[],int,int);
void polygon();



void scanline(point2f point_1, point2f point_2)
{
    int num = point_2.x - point_1.x + 1;
    point2f next_point = point_1;
    for(int i=0; i <= num; i++)
    {
        createPoint(next_point);
        next_point.x++;
        float u1 = fabs((float)(point_2.x-next_point.x)/(point_2.x-point_1.x));
        float u2 = fabs((next_point.x-point_1.x)/(point_2.x-point_1.x));
        next_point.r = u1*point_1.r + u2*point_2.r;
        next_point.g = u1*point_1.g + u2*point_2.g;
        next_point.b = u1*point_1.b + u2*point_2.b;
    }
}

void drawLines(point2f point_1,point2f point_2,point2f *pt)
{
    float delta_y = fabs(point_2.y - point_1.y);
    float delta_x = fabs(point_2.x - point_1.x);
    float m = delta_y/delta_x;
    int flag = 0;

    if((point_2.y - point_1.y)/(point_2.x - point_1.x) > 0)flag = 1;
    else flag = -1;
    point2f next_point = point_1;
    pt[0] = point_1;
    if(m < 1){

        float p = (2*delta_y - delta_x);
        for(int i = 1; i < delta_x + 1;i++)
        {
            if(p < 0){
                next_point.x++;
                p +=(2*delta_y);
            }else{
                next_point.x++;
                next_point.y += flag;
                p += (2*delta_y - 2*delta_x);
            }

            float u1 = fabs((float)(point_2.x-next_point.x)/(point_2.x-point_1.x));
            float u2 = fabs((next_point.x-point_1.x)/(point_2.x-point_1.x));
            next_point.r = u1*point_1.r + u2*point_2.r;
            next_point.g = u1*point_1.g + u2*point_2.g;
            next_point.b = u1*point_1.b + u2*point_2.b;
            pt[i] = next_point;
        }
    }
    else
    {
        float p = (2*delta_x - delta_y);
        for(int i = 1; i < delta_y+1; i++)
        {
            if(p<0){
                next_point.y += flag;
                p += (2*delta_x);
            }
            else
            {
                next_point.x++;
                next_point.y += flag;
                p += (2*delta_x - 2*delta_y);
            }

            float u1 = fabs((float)(point_2.x-next_point.x)/(point_2.x-point_1.x));
            float u2 = fabs((next_point.x-point_1.x)/(point_2.x-point_1.x));
            next_point.r = u1*point_1.r + u2*point_2.r;
            next_point.g = u1*point_1.g + u2*point_2.g;
            next_point.b = u1*point_1.b + u2*point_2.b;
            pt[i] = next_point;
        }
    }

}

void polygon()
{
    drawLines(vertexPts[1],vertexPts[0],pts_1);
    drawLines(vertexPts[1],vertexPts[2],pts_2);
    drawLines(vertexPts[2],vertexPts[3],pts_3);
    drawLines(vertexPts[0],vertexPts[3],pts_4);

    point2f pts0[3*M+2],pts1[4*M+N+2];
    for(int i = 0;i <= M;i++){
        pts0[i] = pts_1[M-i];
    }
    for(int i = 0;i <= 2*M;i++){
        pts1[i] = pts_4[i];
        pts1[2*M+1+i] = pts_3[2*M+N-i];
        pts0[i+M+1] = pts_2[i];
    }
    for(int i = 0;i < N;i++){
        pts1[4*M+2+i] = pts_3[N-1-i];
    }
    paintLine(pts0,pts1,3*M+1,4*M+N+1);
    paintLine(pts_3,pts_4,2*M+N,2*M);
}

void paintLine(point2f pts0[],point2f pts1[],int num1,int num2)
{
    int i=0,j=0;
    while(i<=num1 && j<=num2)
    {
        if(pts0[i].y == pts1[j].y){
            scanline(pts0[i],pts1[j]);
            i++;
            j++;
        }
        else if(pts0[i].y > pts1[j].y)
        {
            createPoint(pts0[i]);
            i++;
        }
        else
        {
            createPoint(pts1[j]);
            j++;
        }
    }
    if(i<=num1)
    {
        while(i<=num1)
        {
            createPoint(pts0[i]);
            i++;
        }
    }
    if(j<=num2)
    {
        while(j<=num2)
        {
            createPoint(pts1[j]);
            j++;
        }
    }
}


void createPoint(point2f pt)
{
    glBegin(GL_POINTS);
    glColor3f(pt.r,pt.g,pt.b);
    glVertex2f(pt.x,pt.y);
    glEnd();
}


void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);
    polygon();
    glFlush();
    glutSwapBuffers();
}

void keyBoard(unsigned char c, int x, int y){
    if(c == 'q' || c == 'Q'){
        exit(0);
    }
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity ( );
    gluOrtho2D(-w/2,w/2,-h/2,h/2);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Fill Lab 10");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyBoard);
    glutMainLoop();
    return EXIT_SUCCESS;
}