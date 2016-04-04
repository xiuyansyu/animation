	glBegin(GL_QUADS);

	int number_of_vertices = chestbelly_vertices.size();
	for (int j = 0; j < number_of_vertices; j++){
		glNormal3f( chestbelly_normals[j].x, chestbelly_normals[j].y, chestbelly_normals[j].z);
		glVertex3f( chestbelly_vertices[j].x, chestbelly_vertices[j].y, chestbelly_vertices[j].z);
	}
/*
		glNormal3f( chestbelly_normals[j+1].x, chestbelly_normals[j+1].y, chestbelly_normals[j+1].z);
		glVertex3f( chestbelly_vertices[j+1].x, chestbelly_vertices[j+1].y, chestbelly_vertices[j+1].z);
		glNormal3f( chestbelly_normals[j+2].x, chestbelly_normals[j+2].y, chestbelly_normals[j+2].z);
		glVertex3f( chestbelly_vertices[j+2].x, chestbelly_vertices[j+2].y, chestbelly_vertices[j+2].z);
		glNormal3f( chestbelly_normals[j+3].x, chestbelly_normals[j+3].y, chestbelly_normals[j+3].z);
		glVertex3f( chestbelly_vertices[j+3].x, chestbelly_vertices[j+3].y, chestbelly_vertices[j+3].z);*/


	number_of_polygons = head_vertices.size() / 4;
	for (int j = 0; j < number_of_polygons; j++){
		glNormal3f( head_normals[j].x, head_normals[j].y, head_normals[j].z);
		glVertex3f( head_vertices[j].x, head_vertices[j].y, head_vertices[j].z);
	}

/*
		glNormal3f( head_normals[j+1].x, head_normals[j+1].y, head_normals[j+1].z);
		glVertex3f( head_vertices[j+1].x, head_vertices[j+1].y, head_vertices[j+1].z);
		glNormal3f( head_normals[j+2].x, head_normals[j+2].y, head_normals[j+2].z);
		glVertex3f( head_vertices[j+2].x, head_vertices[j+2].y, head_vertices[j+2].z);
		glNormal3f( head_normals[j+3].x, head_normals[j+3].y, head_normals[j+3].z);
		glVertex3f( head_vertices[j+3].x, head_vertices[j+3].y, head_vertices[j+3].z);
	}*/

	number_of_polygons = leftleg_vertices.size() / 4;
	for (int j = 0; j < number_of_polygons; j++){
		glNormal3f( leftleg_normals[j].x, leftleg_normals[j].y, leftleg_normals[j].z);
		glVertex3f( leftleg_vertices[j].x, leftleg_vertices[j].y, leftleg_vertices[j].z);
		glNormal3f( leftleg_normals[j+1].x, leftleg_normals[j+1].y, leftleg_normals[j+1].z);
		glVertex3f( leftleg_vertices[j+1].x, leftleg_vertices[j+1].y, leftleg_vertices[j+1].z);
		glNormal3f( leftleg_normals[j+2].x, leftleg_normals[j+2].y, leftleg_normals[j+2].z);
		glVertex3f( leftleg_vertices[j+2].x, leftleg_vertices[j+2].y, leftleg_vertices[j+2].z);
		glNormal3f( leftleg_normals[j+3].x, leftleg_normals[j+3].y, leftleg_normals[j+3].z);
		glVertex3f( leftleg_vertices[j+3].x, leftleg_vertices[j+3].y, leftleg_vertices[j+3].z);
	}

	number_of_polygons = leftarm_vertices.size() / 4;
	for (int j = 0; j < number_of_polygons; j++){
		glNormal3f( leftarm_normals[j].x, leftarm_normals[j].y, leftarm_normals[j].z);
		glVertex3f( leftarm_vertices[j].x, leftarm_vertices[j].y, leftarm_vertices[j].z);
		glNormal3f( leftarm_normals[j+1].x, leftarm_normals[j+1].y, leftarm_normals[j+1].z);
		glVertex3f( leftarm_vertices[j+1].x, leftarm_vertices[j+1].y, leftarm_vertices[j+1].z);
		glNormal3f( leftarm_normals[j+2].x, leftarm_normals[j+2].y, leftarm_normals[j+2].z);
		glVertex3f( leftarm_vertices[j+2].x, leftarm_vertices[j+2].y, leftarm_vertices[j+2].z);
		glNormal3f( leftarm_normals[j+3].x, leftarm_normals[j+3].y, leftarm_normals[j+3].z);
		glVertex3f( leftarm_vertices[j+3].x, leftarm_vertices[j+3].y, leftarm_vertices[j+3].z);
	}

	number_of_polygons = rightleg_vertices.size() / 4;
	for (int j = 0; j < number_of_polygons; j++){
		glNormal3f( rightleg_normals[j].x, rightleg_normals[j].y, rightleg_normals[j].z);
		glVertex3f( rightleg_vertices[j].x, rightleg_vertices[j].y, rightleg_vertices[j].z);
		glNormal3f( rightleg_normals[j+1].x, rightleg_normals[j+1].y, rightleg_normals[j+1].z);
		glVertex3f( rightleg_vertices[j+1].x, rightleg_vertices[j+1].y, rightleg_vertices[j+1].z);
		glNormal3f( rightleg_normals[j+2].x, rightleg_normals[j+2].y, rightleg_normals[j+2].z);
		glVertex3f( rightleg_vertices[j+2].x, rightleg_vertices[j+2].y, rightleg_vertices[j+2].z);
		glNormal3f( rightleg_normals[j+3].x, rightleg_normals[j+3].y, rightleg_normals[j+3].z);
		glVertex3f( rightleg_vertices[j+3].x, rightleg_vertices[j+3].y, rightleg_vertices[j+3].z);
	}

	number_of_polygons = rightarm_vertices.size() / 4;
	for (int j = 0; j < number_of_polygons; j++){
		glNormal3f( rightarm_normals[j].x, rightarm_normals[j].y, rightarm_normals[j].z);
		glVertex3f( rightarm_vertices[j].x, rightarm_vertices[j].y, rightarm_vertices[j].z);
		glNormal3f( rightarm_normals[j+1].x, rightarm_normals[j+1].y, rightarm_normals[j+1].z);
		glVertex3f( rightarm_vertices[j+1].x, rightarm_vertices[j+1].y, rightarm_vertices[j+1].z);
		glNormal3f( rightarm_normals[j+2].x, rightarm_normals[j+2].y, rightarm_normals[j+2].z);
		glVertex3f( rightarm_vertices[j+2].x, rightarm_vertices[j+2].y, rightarm_vertices[j+2].z);
		glNormal3f( rightarm_normals[j+3].x, rightarm_normals[j+3].y, rightarm_normals[j+3].z);
		glVertex3f( rightarm_vertices[j+3].x, rightarm_vertices[j+3].y, rightarm_vertices[j+3].z);
	}

	number_of_polygons = stamp_vertices.size() / 4;
	for (int j = 0; j < number_of_polygons; j++){
		glNormal3f( stamp_normals[j].x, stamp_normals[j].y, stamp_normals[j].z);
		glVertex3f( stamp_vertices[j].x, stamp_vertices[j].y, stamp_vertices[j].z);
		glNormal3f( stamp_normals[j+1].x, stamp_normals[j+1].y, stamp_normals[j+1].z);
		glVertex3f( stamp_vertices[j+1].x, stamp_vertices[j+1].y, stamp_vertices[j+1].z);
		glNormal3f( stamp_normals[j+2].x, stamp_normals[j+2].y, stamp_normals[j+2].z);
		glVertex3f( stamp_vertices[j+2].x, stamp_vertices[j+2].y, stamp_vertices[j+2].z);
		glNormal3f( stamp_normals[j+3].x, stamp_normals[j+3].y, stamp_normals[j+3].z);
		glVertex3f( stamp_vertices[j+3].x, stamp_vertices[j+3].y, stamp_vertices[j+3].z);
	}
*/
	glEnd();
