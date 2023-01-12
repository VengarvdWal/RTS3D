#pragma once
#include <string>
#include <glm/glm.hpp>

#include "btBulletDynamicsCommon.h"

class Model;

class GameObject
{

protected:
	Model* model;
	btRigidBody* body;


private:


	glm::mat4 matrix;
	btTransform transform;
	btScalar mass;
	btDefaultMotionState* motionState;
	

public:

	GameObject();
	GameObject(std::string modelPath, btScalar mass, btVector3 position);
	btVector3 position;
	/*~GameObject();*/

	btRigidBody* getRigidBody();
	btVector3 getTransform();
	btVector3 getRotationAxis();
	btScalar getRotationAngle();

	void Render();
	void Update();
};

