#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <bullet/btBulletDynamicsCommon.h>


glm::vec3 BulletToGlm(const btVector3& v) { return glm::vec3(v.getX(), v.getY(), v.getZ()); }

btVector3 GlmToBullet(const glm::vec3& v) { return btVector3(v.x, v.y, v.z); }

glm::quat BulletToGlm(const btQuaternion& q) { return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ()); }

btQuaternion GlmToBullet(const glm::quat& q) { return btQuaternion(q.x, q.y, q.z, q.w); }

