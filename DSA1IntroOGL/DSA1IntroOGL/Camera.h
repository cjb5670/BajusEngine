#pragma once
#include <glm/glm.hpp>
#include "Object.h"
#include <vector>
using namespace glm;
class Camera
{
public:
	float fovy;
	float aspect;
	float zNear;
	float zFar;

	vec3 camLoc;
	vec3 camRot;
	vec3 camVel;

	Object * follow;

	float sens;
	int w, h;
	double x, y;
	vec3 eye;
	vec3 center;
	vec3 up;
	
	mat3 rotMat;
	float speed;

	mat4 lookAtMat;
	mat4 perspecMat;
	mat4 worldViewMat;

	float t = 0;
	
	Camera();
	Camera(Object &_follow);
	void Rotation();
	void Move();
	void Update();
	bool testMode;
	~Camera();
};


