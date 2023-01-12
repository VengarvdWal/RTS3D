#pragma once
#include <GLES3/gl3.h>
#include <vector>
#include "Mesh.h"


class Terrain
{
public:
	Terrain();

	Mesh* mesh;

	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	void CreateTerrain(unsigned int w, unsigned int h);
	void RenderTerrain();
	void CalculateNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
		unsigned int vLength, unsigned int normalOffset);

};

