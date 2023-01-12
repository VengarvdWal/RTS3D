#include "DebugDrawer.h"
#include "GLES3/gl3.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>


#include "GLES3/gl3ext.h"
#include "OpenGLES.h"


GLuint VAO, VBO;

GLuint projection, view;

DebugDrawer::DebugDrawer() : btIDebugDraw()
{
	
}



void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{

	/*const float vertexPositions[] = {
	from[0], from[1], from[2], 1,
	to[0], to[1], to[2], 1 };*/
    LineValues Line;
    Line.p1 = glm::vec3(from.x(), from.y(), from.z());
	//Line.color = glm::vec3(color.x(), color.y(), color.z());
	LineValues Line2;
	Line2.p1 = glm::vec3(to.x(), to.y(), to.z());
	//Line.color = glm::vec3(color.x(), color.y(), color.z());
	
    //Line.p2 = glm::vec3(to.x(), to.y(), to.z());

    TheLines.push_back(Line);
	TheLines.push_back(Line2);


	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUniformMatrix4fv(projection, 1, false, glm::value_ptr(projectionMat));
	glUniformMatrix4fv(view, 1, false, glm::value_ptr(viewMat));

	const float vertexPositions[] = {
	from[0], from[1], from[2], 1,
	to[0], to[1], to[2], 1 };
	int sz = sizeof(vertexPositions);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sz, vertexPositions);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);*/


}


//void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
//{
//	// Vertex data
//	
//
//	points[0] = from.x();
//	points[1] = from.y();
//	points[2] = from.z();
//	points[3] = color.x();
//	points[4] = color.y();
//	points[5] = color.z();
//
//	points[6] = to.x();
//	points[7] = to.y();
//	points[8] = to.z();
//	points[9] = color.x();
//	points[10] = color.y();
//	points[11] = color.z();
//
//}

void DebugDrawer::DebugDraw()
{
	

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, TheLines.size() * 12, &TheLines[0], GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);	

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	
	glDrawArrays(GL_LINES, 0, (TheLines.size()));

	TheLines.clear();
}

void DebugDrawer::DebugDraw(GLuint uniformProjection, GLuint uniformView, glm::mat4 pProjection, glm::mat4 pView)
{
	

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, TheLines.size() * 12, &TheLines[0], GL_STATIC_DRAW);

	glUniformMatrix4fv(projection, 1, false, glm::value_ptr(pProjection));
	glUniformMatrix4fv(view, 1, false, glm::value_ptr(pView));
	

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glDrawArrays(GL_LINES, 0, TheLines.size());	

	TheLines.clear();

}

//
//void DebugDrawer::DebugDraw(GLuint ProgramObject)
//{
//    if (TheLines.size() == 0) return;
//    //glUseProgram(ProgramObject);
//    int howmany = TheLines.size();
//
//
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0); 
//    GLuint positionLoc = glGetUniformLocation(ProgramObject, "vPos");
//    GLuint ColourLoc = glGetUniformLocation(ProgramObject, "fragColor");
//
//    int stride = 8 * sizeof(float);  // 4 floats for each btvec3
//
//    glVertexAttribPointer(positionLoc,3,GL_FLOAT,GL_FALSE, stride, (GLfloat*)&TheLines[0]);
//        
//    glEnableVertexAttribArray(positionLoc);
//    glEnableVertexAttribArray(ColourLoc);
//
//    glDrawArrays(GL_LINES, 0, (TheLines.size() * 2)); // there are 2 componants to send
//
//    //TheLines.clear();
//}


