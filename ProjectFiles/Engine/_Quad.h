#ifndef _QUAD_H
#define _QUAD_H

#include "glfw3.h"

//Quad struct, it stores the quad vertices and indices information, and some helpful variables
struct _Quad
{
	const GLfloat vertices[12] =
	{
		-0.5, -0.5, 0,
		 0.5, -0.5, 0,
		 0.5,  0.5, 0,
		-0.5,  0.5, 0
	};

	const GLubyte indices[6] =
	{
		0,1,2, 2,3,0
	};

	const GLenum renderMode = GL_TRIANGLES;

	const GLenum vertexDataType = GL_FLOAT;
	const GLsizei vertexSize = sizeof(GLfloat) * 3;

	const GLenum indexDataType = GL_UNSIGNED_BYTE;
	const GLsizei indexCount = 6;
};


#endif
