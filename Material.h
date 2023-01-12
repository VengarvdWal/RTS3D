#pragma once
class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat roughness);

	void UseMaterial(GLuint specularIntensityLocation, GLuint roughnessLocation);


	~Material();

private:
	GLfloat specularIntensity;
	GLfloat roughness;

};

