#include "Terrain.h"
#include "PerlinNoise.hpp"
#include <glm/glm.hpp>
#include "Mesh.h"


Terrain::Terrain()
{
	
}

void Terrain::CreateTerrain(unsigned int w, unsigned int h)
{
	vertices.reserve(w * h);

	float spacing = 1.0f;

	const siv::PerlinNoise::seed_type seed = 12356u;
	const siv::PerlinNoise perlin{ seed };
	float noise;

	for (int r = 0; r < h; r++)
	{
		for (int c = 0; c < w; c++)
		{
			float x = c;
			float z = r;
			noise = perlin.octave2D_01((c), (r), 8);
			float y = noise * 0.75f;

			glm::vec3 v = glm::vec3(x, y, z);
			
			vertices.insert(vertices.end(), { v.x, v.y, v.z });
			vertices.insert(vertices.end(), { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
			
		}
	}

	for (int r = 0; r < h - 1; r++)
	{
		for (int c = 0; c < w - 1; c++)
		{
			// Upper triangle
			/*

				v0 -- v2
				|    /
				|  /
				v1
			*/
			uint f0_0 = (r * w) + c;
			uint f0_1 = ((r + 1) * w) + c;
			uint f0_2 = (r * w) + c + 1;

			indices.insert(indices.end(), { f0_0, f0_1, f0_2 });

			//// Lower triangle
	   /*

				 v2
				/ |
			  /   |
		   v0 --- v1
	   */
			uint f1_0 = ((r + 1) * w) + c;
			uint f1_1 = ((r + 1) * w) + c + 1;
			uint f1_2 = (r * w) + c + 1;
	
			indices.insert(indices.end(), { f1_0, f1_1, f1_2 }); 
		}
	}

	CalculateNormals(&indices[0], indices.size(), &vertices[0], vertices.size(), 8, 5);

	mesh = new Mesh();
	mesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	
}

void Terrain::CalculateNormals(uint* indices, uint indiceCount, GLfloat* vertices, uint verticeCount, 
						uint vLength, uint normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		uint in0 = indices[i] * vLength;
		uint in1 = indices[i + 1] * vLength;
		uint in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;		
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		uint nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void Terrain::RenderTerrain()
{
	mesh->RenderMesh();
}

