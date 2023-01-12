#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
//#include <string>

#include "Shader.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformVP = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
	
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation, bool aDebug)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	debug = aDebug;


	CompileShader(vertexCode, fragmentCode);

	
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	uint vertexShader = AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	uint fragmentShader = AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	if (debug)
	{

		uniformVP = glGetUniformLocation(shaderID, "VP");
	}


	if (!debug)
	{
		uniformProjection = glGetUniformLocation(shaderID, "projection");
		uniformView = glGetUniformLocation(shaderID, "view");
		uniformModel = glGetUniformLocation(shaderID, "model");
		uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "dLight.base.color");
		uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "dLight.base.ambientIntensity");
		uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "dLight.direction");
		uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "dLight.base.diffuseIntensity");
		uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
		uniformRoughness = glGetUniformLocation(shaderID, "material.roughness");
		uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

		uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
		for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
		{
			char locBuff[100] = { '\0' };
			snprintf(locBuff, sizeof(locBuff), "pLights[%d].base.color", i);
			uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "pLights[%d].base.ambientIntensity", i);
			uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "pLights[%d].base.diffuseIntensity", i);
			uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "pLights[%d].position", i);
			uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "pLights[%d].constant", i);
			uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "pLights[%d].linear", i);
			uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "pLights[%d].quadratic", i);
			uniformPointLight[i].uniformQuadratic = glGetUniformLocation(shaderID, locBuff);
		}

		uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");


		for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
		{
			char locBuff[100] = { '\0' };
			snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.base.color", i);
			uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.base.ambientIntensity", i);
			uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.base.diffuseIntensity", i);
			uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.position", i);
			uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.constant", i);
			uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.linear", i);
			uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "sLights[%d].base.quadratic", i);
			uniformSpotLight[i].uniformQuadratic = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "sLights[%d].direction", i);
			uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

			snprintf(locBuff, sizeof(locBuff), "sLights[%d].edge", i);
			uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);


		}
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	
}


GLuint Shader::GetVPLocation()
{
	return uniformVP;
}


GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}
GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

GLuint Shader::GetViewLocation()
{
	return uniformView;
}

GLuint Shader::GetAmbientColorLocation()
{
	return uniformDirectionalLight.uniformColor;
}

GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}

GLuint Shader::GetRoughnessLocation()
{
	return uniformRoughness;
}

GLuint Shader::GetEyePositionLocation()
{
	return uniformEyePosition;
}

GLuint Shader::GetShaderID()
{
	return shaderID;
}


void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformQuadratic);
	}
}

void Shader::SetSpotLights(SpotLight* sLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformQuadratic,
			uniformSpotLight[i].uniformEdge);
	}
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}



void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}


unsigned int Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return 0;
	}

	glAttachShader(theProgram, theShader);

	return theShader;
}

Shader::~Shader()
{
	ClearShader();
}











