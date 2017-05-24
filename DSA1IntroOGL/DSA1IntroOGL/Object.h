#pragma once
#include <FreeImage.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <vector>
#include <string>

#include "Transform.h"
#include "Model.h"
class Object
{
public:
	Object();
	enum collider
	{
		colliderless, AABB, sphere
	};
	collider colliderType;
	Object(const char* texName, const char* modelName, GLFWwindow* objectWindowPtr, collider collidertype);
	~Object();
	void update();
	void setTransforms(vec3 _location, vec3 _rotation, vec3 _size);
	void unload();
	//void findSize();
	bool collidesWith(const Object &_object);
	Transform transform;
	std::string texFileName;
	bool isDisabled;
	bool isCaptured;
	

	// Used to make old code function
	Model model;
	unsigned int baseTexID;
};

