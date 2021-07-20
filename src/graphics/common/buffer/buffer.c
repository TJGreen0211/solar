#include "buffer.h"

void buffer_init_object(buffer_t *buf_object) {
    glGenVertexArrays(1, &buf_object->vao);
	glBindVertexArray(buf_object->vao);
    glGenBuffers(1, &buf_object->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, buf_object->vbo);

	printf("ASDF QWERTY\n");

    glBufferData(GL_ARRAY_BUFFER, buf_object->point_size+buf_object->normal_size+buf_object->tangent_size+buf_object->tex_coord_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, buf_object->point_size, buf_object->points);
	glBufferSubData(GL_ARRAY_BUFFER, buf_object->point_size, buf_object->normal_size, buf_object->normals);
	glBufferSubData(GL_ARRAY_BUFFER, buf_object->point_size+buf_object->normal_size, buf_object->tangent_size, buf_object->tangents);
	glBufferSubData(GL_ARRAY_BUFFER, buf_object->point_size+buf_object->normal_size+buf_object->tangent_size, buf_object->tex_coord_size, buf_object->tex_coords);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), BUFFER_OFFSET(buf_object->point_size));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), BUFFER_OFFSET(buf_object->point_size+buf_object->normal_size));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), BUFFER_OFFSET(buf_object->point_size+buf_object->normal_size+buf_object->tangent_size));
	glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	buf_object->vertex_number = (buf_object->point_size/sizeof(float))/3;

	free(buf_object->points);
	free(buf_object->normals);
	free(buf_object->tangents);
	free(buf_object->tex_coords);
}