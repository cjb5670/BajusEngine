#pragma once
#include <FreeImage.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <vector>
#include <string>
using namespace glm;

class Transform
{
public:
	Transform();
	~Transform();
	// Vecs for basic Transform updates
	vec3 location;
	vec3 rotation;
	mat3 rotMat;
	vec3 size;
	mat4 objectWorld;

	// Vecs for force-based movement
	vec3 position;
	vec3 prevDirection;
	vec3 direction;
	vec3 acceleration;
	vec3 velocity;

	// floats for forces
	float mass;
	float maxSpeed;

	// floats for wandering
	float jitter;
	float turnRadius;
	float wanderAngle;

	// differentiates between friend and enemy
	void updateFriend(float _dt);
	void updateEnemy(float _dt);
	void updateMissile(float _dt);

	// details what needs to be done in an update
	void ApplyForce(vec3 force);
	void ApplyRotate(float yaw, float pitch, float roll, float dt);
	void UpdatePosition(float _dt);
	void UpdateTransform(float _dt);
	void ApplyFriction(float coeff);

	// Specifics for friends and enmiens.
	vec3 Wandering();
	void CameraFollow(float dt);
	bool MissileDecay(float dt, float timeTillDeath);
	float destroyTimer = 0;

	// Set Functions
	void SetForces(float _mass, float _maxSpeed);
	void SetWander(float _jitter, float _turnRadius, float _wanderAngle);
};

