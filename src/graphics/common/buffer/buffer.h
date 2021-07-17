#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <stdio.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

typedef struct buffer_t {
    unsigned int vao;
    unsigned int vbo;
    int vertex_number;
    int point_size;
    int normal_size;
    int tangent_size;
    int tex_coord_size;
    float *points[3];
	float *normals[3];
	float *tangents[3];
	float *tex_coords[2];
} buffer_t;

#define BUFFER_OFFSET(offset) ((char*)NULL+(offset))

void buffer_init_object(buffer_t *buf_object);

#endif