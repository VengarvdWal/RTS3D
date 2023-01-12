#include "GameObject.h"
#include "Model.h"
#include <glm/glm.hpp>

#include <glm/gtc/type_ptr.hpp>

GameObject::GameObject()
{
	
}


GameObject::GameObject(std::string modelPath, btScalar mass, btVector3 position)
{
	matrix = glm::mat4(1.0);
	model = new Model();
	model->LoadModel(modelPath);	
	this->mass = mass;
	this->position = position;

	btBoxShape* boxShape = new btBoxShape(btVector3(3.0f, 2.5f, 6.5f));

	transform.setIdentity();
	transform.setOrigin(btVector3(this->position));

	btVector3 bodyInertia{};
	boxShape->calculateLocalInertia(1.0, bodyInertia);

	motionState = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(1.0, motionState, boxShape, bodyInertia);
	body = new btRigidBody(rbInfo);
	

}


btRigidBody* GameObject::getRigidBody()
{
	return body;
}

btVector3 GameObject::getTransform()
{	
	return body->getWorldTransform().getOrigin();
}

btVector3 GameObject::getRotationAxis()
{
	
	
	return body->getWorldTransform().getRotation().getAxis();
}

btScalar GameObject::getRotationAngle()
{

	return body->getWorldTransform().getRotation().getAngle();
}

void GameObject::Render()
{
	model->RenderModel();
}

void GameObject::Update()
{

	//body->getWorldTransform().setRotation(btQuaternion(0.0f, 0.5f, 1.0f, 1.0f));
	//body->applyCentralForce(btVector3(1, 0, 0));
	//printf("X: %f, Y: %f, Z: %f\n", transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ());
	//printf("X: %f, Y: %f, Z: %f\n", getTransform().getX(), getTransform().getY(), getTransform().getZ());
}

//
//GameObject::~GameObject()
//{
//	
//}


