#pragma once
#include "btBulletDynamicsCommon.h"
class PhysicsManager
{
private:
	btDiscreteDynamicsWorld* dynamicsWorld;

public:

	PhysicsManager();
	~PhysicsManager();

	void Update(const float timeStep);
	



};

