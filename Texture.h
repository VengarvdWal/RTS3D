#pragma once
#include <GLES3/gl3.h>


#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* filePath);

	bool LoadTexture();
	bool LoadTextureA();



	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* filePath;
};
