#include "GLES3/gl3.h"

#include "Material.h"


Material::Material()
{
	specularIntensity = 0.0f;
	roughness = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat roughness)
{
	specularIntensity = sIntensity;
	this->roughness = roughness;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint roughnessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(roughnessLocation, roughness);
}

Material::~Material()
{
	
}


