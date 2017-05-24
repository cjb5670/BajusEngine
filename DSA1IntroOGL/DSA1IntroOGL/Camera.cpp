#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>


Camera::Camera()
{
	
}

Camera::Camera(Object &_follow)
{
	testMode = false;
	follow = &_follow;
	// Zoom or change aspect ratio
	float zoom = 1.f;
	int width = 800;
	int height = 600;

	float fovy = 3.14159f * .4f / zoom;
	float aspect = (float)width / (float)height;
	float zNear = .01f;
	float zFar = 1000.f;

	perspecMat = perspective(fovy, aspect, zNear, zFar);

	// rotation varibles
	camLoc = { 0, 0, 2 };
	camRot = { 0, 0, 0 };
	sens = .005;
	w = 800;
	h = 600;
	glfwGetWindowSize(glfwGetCurrentContext(), &w, &h);
	glfwSetCursorPos(glfwGetCurrentContext(), w / 2, h / 2);
	speed = 1.f;
}

void Camera::Rotation()
{

	// FPS Controls
	// turn with mouse
	//glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);

	//camRot.y -= sens * (x - w *.5f); // yaw
	//camRot.x -= sens * (y - h *.5f); // pitch
	//camRot.x = clamp(camRot.x, -.5f * (float)M_PI, .5f * (float)M_PI);

	//glfwSetCursorPos(glfwGetCurrentContext(), w * .5f, h *.5f);
	////Look-At Matrix
	//rotMat = (mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);
	
	// eye = camLoc;
	// Follows object attached to. Rotates on the x axis, but not Y or Z.
	mat3 tempRotMat = (mat3)glm::yawPitchRoll(follow->transform.rotation.x + 1.57f, 0.0f, 0.0f);
	eye = follow->transform.position + tempRotMat*vec3(0, .15, .5);
	center = eye + tempRotMat * glm::vec3(0, 0, -1);
	up = tempRotMat * glm::vec3(0, 1, 0);
	
	lookAtMat = glm::lookAt(eye, center, up);
	
}

void Camera::Move()
{


	float t0 = t;
	t = (float)glfwGetTime();
	float dt = t - t0;

	camVel = vec3();

	
	if (testMode)
	{
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A))
		{
			camVel += rotMat * vec3(-1, 0, 0);
		}
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D))
		{
			camVel += rotMat * vec3(1, 0, 0);
		}
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W))
		{
			camVel += rotMat * vec3(0, 0, -1);
		}
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S))
		{
			camVel += rotMat * vec3(0, 0, 1);
		}

		if (camVel != vec3())
		{
			camVel = normalize(camVel) * speed;
		}
	}
	else
	{
		camLoc = follow->transform.location + vec3(0, .075, 0.35);
	}

	
	// scale by time
	camLoc += camVel * dt;
}

void Camera::Update()
{
	Rotation();
	Move();
	worldViewMat = perspecMat * lookAtMat;
	// update shader
	glUniformMatrix4fv(2, 1, GL_FALSE, &worldViewMat[0][0]);
	glUniform3f(5, eye.x, eye.y, eye.z);
}

Camera::~Camera()
{
}

