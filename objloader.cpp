/***********************************************************
             CSC418, Winter 2016
 
                 objloader.cpp
                 author: Xiuyan Yu

	For loading OBJ file downloaded from the internet

	Some code segments copied from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/




***********************************************************/
#include "objloader.h"

/* NOTE: loadOBJ parses and loads from a file. therefore each object (e.g. body part must be 		described in a separate file. 
*/


bool loadOBJ(
    char * path,
    std::vector < Vertex > & out_vertices,
    std::vector < UV > & out_uvs,
    std::vector < Normal > & out_normals,
    unsigned int first_index_v,
    unsigned int first_index_vn,
    unsigned int first_index_vt
) {
	std::vector < unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector < Vertex > temp_vertices;
	std::vector < UV > temp_uvs;
	std::vector < Normal > temp_normals;

	FILE * file = fopen(path, "r");
	if( file == NULL ){
	    printf("Error cannot open file\n");
	    return false;
	}

	while( 1 ){

		char lineHeader[128];     // read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop. 	

		else {  // parse lineHeader

			// parse verteces
			if ( strcmp( lineHeader, "v" ) == 0 ){
			    Vertex vertex;
			    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			    temp_vertices.push_back(vertex);
			}

			// parse texture info
			else if ( strcmp( lineHeader, "vt" ) == 0 ){
			    UV uv;
			    fscanf(file, "%f %f\n", &uv.x, &uv.y );
			    temp_uvs.push_back(uv);
			}

			// parse normals
			else if ( strcmp( lineHeader, "vn" ) == 0 ){
			    Normal normal;
			    fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			    temp_normals.push_back(normal);
			}

			// f
			else if ( strcmp( lineHeader, "f" ) == 0 ){

			    unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
			    fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] , &vertexIndex[3], &uvIndex[3], &normalIndex[3] );
/*			    if (matches != 12){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			    }*/

			    vertexIndices.push_back(vertexIndex[0]-first_index_v);
			    vertexIndices.push_back(vertexIndex[1]-first_index_v);
			    vertexIndices.push_back(vertexIndex[2]-first_index_v);
			    vertexIndices.push_back(vertexIndex[3]-first_index_v);
			    uvIndices    .push_back(uvIndex[0]-first_index_vt);
			    uvIndices    .push_back(uvIndex[1]-first_index_vt);
			    uvIndices    .push_back(uvIndex[2]-first_index_vt);
			    uvIndices    .push_back(uvIndex[3]-first_index_vt);
			    normalIndices.push_back(normalIndex[0]-first_index_vn);
			    normalIndices.push_back(normalIndex[1]-first_index_vn);
			    normalIndices.push_back(normalIndex[2]-first_index_vn);
			    normalIndices.push_back(normalIndex[3]-first_index_vn);
			} // end of if-else
		}

	} // end of while(1)



	for( unsigned int i=0; i<vertexIndices.size(); i++ ) {
		unsigned int vertexIndex = vertexIndices[i];
		Vertex vertex = temp_vertices[ vertexIndex-1 ]; // C++ indexing with 0, OBJ indexing with 1
		out_vertices.push_back(vertex);
	}
/*
	for( unsigned int i=0; i<uvIndices.size(); i++ ) {
		unsigned int uvIndex = uvIndices[i];
		UV uv = temp_uvs[ uvIndex-1 ];
		out_uvs.push_back(uv);
	}
*/
	for( unsigned int i=0; i<normalIndices.size(); i++ ) {
		unsigned int normalIndex = normalIndices[i];
		Normal normal = temp_normals[ normalIndex-1 ];
		out_normals.push_back(normal);
	}
	fclose(file);
	return true;

}
