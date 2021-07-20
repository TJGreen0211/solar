#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <stdio.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define BUFFER_OFFSET(offset) ((char*)NULL+(offset))

typedef struct buffer_t {
    unsigned int vao;
    unsigned int vbo;
    int vertex_number;
    int point_size;
    int normal_size;
    int tangent_size;
    int tex_coord_size;
    float *points;
	float *normals;
	float *tangents;
	float *tex_coords;
} buffer_t;

void buffer_init_object(buffer_t *buf_object);

#endif