#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <math.h>
#include <glm.hpp>
#include <algorithm>
#include <sstream>

struct Model {

	jvertex *vertices;
	vector<unsigned int> indices;
	vec3 mass_center;
	vec3 pos;
	quat rotation;
	Transform transform;
	size_t vertices_size;
	vec3 line_color;

	unsigned int index_buffer;
	unsigned int vertex_buffer;

	void init(size_t size)
	{
		this->vertices = (jvertex*)my_malloc(size, sizeof(jvertex));
		//this->offscreen_vertices = (jvertex*)my_malloc(size, sizeof(jvertex));
		this->vertices_size = size;
	}
	void init_gl_buffers()
	{
		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glGenBuffers(1, &vertex_buffer);
	}
};

struct JModel {
	object data;
	vec3 mass_center;
	vec3 local_position;
	vec3 world_position;
	quat rotation;
	mat4 mat_model = {};
	size_t vertices_size;
	vec3 line_color;
	vec3 front;
	vec3 f;
	vec3 r;
	vec3 u;
	vec3 world_up = vec3_up;
	Transform transform;
	unsigned int vertex_buffer;

	void init_gl_buffers()
	{
		int sz = data.beams.size();
		for (int i = 0; i < sz; i++)
		{
			glGenBuffers(1, &data.beams[i].index_buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.beams[i].index_buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*data.beams[i].indices.size(), &data.beams[i].indices[0], GL_STATIC_DRAW);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glGenBuffers(1, &vertex_buffer);
	}


	void translate(vec3 translation)
	{
		glm::translate(mat_model, translation);
	}

	void uniformScale(float scale)
	{
		glm::scale(mat_model, vec3(scale, scale, scale));
	}
	
	void rotate(vec3 axis, float angle)
	{
		glm::rotate(mat_model, angle, axis);
	}

	vec3 worldPosition()
	{
		 return vec3(mat_model[3][0], mat_model[3][1], mat_model[3][2]);
	}

	vec3 forward()
	{
		return this->f = vec3(mat_model[2][0], mat_model[2][1], mat_model[2][2]);
	}
	vec3 up()
	{
		return this->u = vec3(mat_model[1][0], mat_model[1][1], mat_model[1][2]);
	}
	vec3 right()
	{
		return this->r = vec3(mat_model[0][0], mat_model[0][1], mat_model[0][2]);
	}

};
