
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector.h>
#include <math.h>

#include "keyframe.h"
#include "timer.h"
#include "vector.h"
#include "objloader.h"


std::vector < Vertex > vertices;
std::vector < UV > uvs;
std::vector < Normal > normals;
float elephantrot = 0;


    //other functions and main

    void reshape(int w,int h)
    {
        glViewport(0,0,w,h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
        //glOrtho(-25,25,-2,2,0.1,100);
        glMatrixMode(GL_MODELVIEW);
    }
    void drawBaymax()
    {
        glPushMatrix();
        glTranslatef(0,-40.00,-105);

        glRotatef(elephantrot,0,1,0);


		glBegin(GL_QUADS);

			int number_of_polygons = vertices.size() / 4;
			for (int j = 0; j < number_of_polygons; j++){
				glNormal3f( normals[j].x, normals[j].y, normals[j].z);
				glVertex3f( vertices[j].x, vertices[j].y, vertices[j].z);
				glNormal3f( normals[j+1].x, normals[j+1].y, normals[j+1].z);
				glVertex3f( vertices[j+1].x, vertices[j+1].y, vertices[j+1].z);
				glNormal3f( normals[j+2].x, normals[j+2].y, normals[j+2].z);
				glVertex3f( vertices[j+2].x, vertices[j+2].y, vertices[j+2].z);
				glNormal3f( normals[j+3].x, normals[j+3].y, normals[j+3].z);
				glVertex3f( vertices[j+3].x, vertices[j+3].y, vertices[j+3].z);
			}

		glEnd();



        glPopMatrix();
        elephantrot=elephantrot+0.1;
        if(elephantrot>360)elephantrot=elephantrot-360;
    }
    void display(void)
    {
        glClearColor (0.0,0.0,0.0,1.0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        drawBaymax();
        glutSwapBuffers(); //swap the buffers
    }


    int main(int argc,char **argv)
    {
        glutInit(&argc,argv);
        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
        glutInitWindowSize(800,450);
        glutInitWindowPosition(20,20);
        glutCreateWindow("ObjLoader");
        glutReshapeFunc(reshape);
        glutDisplayFunc(display);
        glutIdleFunc(display);
        loadOBJ("baymax/rightarm.obj", vertices, uvs, normals);
        glutMainLoop();
        return 0;
    }
