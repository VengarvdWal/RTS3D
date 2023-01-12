#pragma once
#include <LinearMath/btIDebugDraw.h>
#include <vector>
#include <glm/glm.hpp>
#include <GLES3/gl3.h>

class Shader;

struct
{
	//btVector3 p1;
	//btVector3 p2;

	glm::vec3 p1;
	//glm::vec3 p2;
	//glm::vec3 color;


}typedef LineValues;


class DebugDrawer : public btIDebugDraw
{

public:


	GLfloat points[12];
	DebugDrawer();

	virtual void setDebugMode(int debugMode) override { m_debugMode = debugMode; }
	virtual int getDebugMode() const override { return m_debugMode; }


	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override{}
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	virtual void reportErrorWarning(const char*) override {}
	virtual void draw3dText(const btVector3&, const char*) override {}

	void DebugDraw();
	void DebugDraw(GLuint uniformProjection, GLuint uniformView, glm::mat4 pProjection, glm::mat4 pView);
		
	
	std::vector<LineValues> TheLines;
	std::vector<float> vertices;
	
	
protected:

	int m_debugMode;

};

