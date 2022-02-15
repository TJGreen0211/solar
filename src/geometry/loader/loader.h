#ifndef LOADER_H
#define LOADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../../utility/matrixmath/matrixMath.h"

typedef struct geometry {
	int vertex_number;
	int point_size;
	int normal_size;
	int tangent_size;
    int tex_coord_size;
    vec3 *points;
	vec3 *normals;
	vec3 *tangents;
	vec2 *tex_coords;
} geometry;


void geometry_load_obj_file(const char *fname, geometry *mesh);
void geometry_load_obj_file_depricated(const char *fname, geometry *mesh);
void geometry_mesh_dealloc(geometry *mesh);

#endif