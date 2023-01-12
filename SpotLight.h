#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
public:
    SpotLight();
    ~SpotLight();

    SpotLight(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat xDir, GLfloat yDir, GLfloat zDir,
        GLfloat con, GLfloat lin, GLfloat quad,
        GLfloat edg);


    void UseLight(GLuint aIntensityLocation, GLuint aColorLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint quadraticLocation,
        GLuint edgeLocation);

private:
    glm::vec3 direction;

    GLfloat edge, procEdge;

};

