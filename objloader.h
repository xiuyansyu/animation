/***********************************************************
             CSC418, Winter 2016
 
                 objloader.cpp
                 author: Xiuyan Yu

	Header file for objloader.cpp

***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glui.h>

struct Normal {		// vn
     GLfloat x, y, z;
};

struct Vertex {		// v
     GLfloat x, y, z;
};

struct UV { 		// vt
     GLfloat x, y;
};


/*

struct Face {		// f gives indeces of verteces that form faces
     int v1,v2,v3,v4, vt1,vt2,vt3,vt4, vn1,vn2,vn3,vn4;
     char type; // ie s, maybe ignore for now???????????????????????
};
*/

/* bool loadOBJ
   read file from "path" and write object data into out_vertices, out_uvs, out_normals
   returns false if unsuccessful
*/
bool loadOBJ(
    char * path,
    std::vector < Vertex > & out_vertices,
    std::vector < UV > & out_uvs,
    std::vector < Normal > & out_normals,
    unsigned int first_index_v,
    unsigned int first_index_vn,
    unsigned int first_index_vt
);


