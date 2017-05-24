#include "Object.h"
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "Shader.h"
#include "Model.h"



Object::Object(const char* texName, const char* modelName, GLFWwindow* objectWindowPtr, collider _collidertype)
{
	// set enum;
	colliderType = _collidertype;
	// functioning
	isDisabled = false;
	isCaptured = false;
	// Loading shader
	Shader shader = Shader();
	if (!shader.load())
	{
		std::cout << "Press Enter to Exit...";
		std::cin.get();
		return;
	}
	else
	{
		shader.use();
	}


	// Loading model
	model = Model();
	model.buffer(modelName);

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetInputMode(objectWindowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Loading texture
	FIBITMAP * image = FreeImage_Load((FreeImage_GetFileType(texName, 0)), texName);
	// Print message and quit if failed
	if (image == nullptr)
	{
		std::cout << "Texture failed to load \n";
		std::cout << "Press Enter to Exit...";
		std::cin.get();
		return;
	}
	// convert to 32 bits
	FIBITMAP * image32Bit = FreeImage_ConvertTo32Bits(image);
	// unload original
	FreeImage_Unload(image);
	// set location and bind
	glGenTextures(1, &baseTexID);
	glBindTexture(GL_TEXTURE_2D, baseTexID);
	// upload texture bytes
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_SRGB_ALPHA, FreeImage_GetWidth(image32Bit), FreeImage_GetHeight(image32Bit), 0,
		GL_BGRA, GL_UNSIGNED_BYTE, (void*)(FreeImage_GetBits(image32Bit)));

	// sets min filter 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// clear from RAM
	FreeImage_Unload(image32Bit);
	
	//findSize();

}

void Object::update()
{
	//glBindTexture(same parameter, texture index to bind)
	glBindTexture(GL_TEXTURE_2D, baseTexID);
	// Sets Transform matrix
	transform.objectWorld = (translate(transform.location) *
		yawPitchRoll(transform.rotation.x, transform.rotation.y, transform.rotation.z) *
		scale(transform.size));
	// Sends to vshader
	glUniformMatrix4fv(6, 1, GL_FALSE, &transform.objectWorld[0][0]);
	model.render();
}

void Object::setTransforms(vec3 _location, vec3 _rotation, vec3 _size)
{
	transform.location = _location;
	transform.rotation = _rotation;
	transform.size = _size;
}

Object::Object()
{

}

void Object::unload()
{
	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &baseTexID);
}

//void Object::findSize()
//{
//	vec3 Maximum = vec3(model.xMax, model.yMax, model.zMax);
//	vec3 Minimum = vec3(model.xMin, model.yMin, model.zMin);
//	vec3 Center = (Maximum + Minimum)/vec3(2,2,2);
//	transform.size = Center + Maximum;
//}

bool Object::collidesWith(const Object & _object)
{
	if (colliderType == colliderless || _object.colliderType == colliderless)
	{
		return false;
	}
	// sphere collider
	else if (colliderType == sphere && _object.colliderType == sphere)
	{
		// calculate distance
		float Arad = transform.size.x;
		float Brad = _object.transform.size.x;
		float radSum = (Arad + Brad) * 10;
		float objDistance = distance(transform.position, _object.transform.position);
		// check distance
		if (objDistance > radSum)
		{
			return false;
		}
		else return true;
	}

	else if (colliderType == AABB && _object.colliderType == AABB)
	{
		// calculate sums
		float objDistance = distance(transform.position, _object.transform.position);
		if (objDistance > (transform.size.x + _object.transform.size.x) * 20)
		{
			return false;
		}
		else if (objDistance > (transform.size.y + _object.transform.size.y) * 10)
		{
			return false;
		}
		else if (objDistance > (transform.size.z + _object.transform.size.z) * 10)
		{
			return false;
		}
		else return true;
	}
	// This is AABB
	else if ((colliderType == sphere && _object.colliderType == AABB))
	{
		// distance from x to sphere center is greater than sphere radius
		//if (distance((transform.position.x + transform.size.x), _object.transform.position.x)*20 > _object.transform.size.x)
		//{
		//	return false;
		//}
		//// distance from y to sphere center is greater than sphere size
		//else if (distance((transform.position.y + transform.size.y), _object.transform.position.y) * 10 > _object.transform.size.x)
		//{
		//	return false;
		//}
		//// distance from z to sphere center is greater than sphere size
		//else if (distance((transform.position.z + transform.size.z), _object.transform.position.z) * 10 > _object.transform.size.x)
		//{
		//	return false;
		//}
		float x = max(-_object.transform.size.x * 100 + _object.transform.position.x, min(transform.position.x, _object.transform.size.x + _object.transform.position.x * 100));
		float y = max(-_object.transform.size.y * 100 + _object.transform.position.y, min(transform.position.y, _object.transform.size.y + _object.transform.position.y * 100));
		float z = max(-_object.transform.size.z * 100 + _object.transform.position.z, min(transform.position.z, _object.transform.size.z + _object.transform.position.z * 100));

		float objDistance = sqrt(pow((x - transform.position.x), 2) +
			pow((y - transform.position.y), 2) +
			pow((z - transform.position.z), 2));

		if (objDistance < _object.transform.size.x * 200) 
		{
			return true;
		}
		else return false;
	}
	// This is sphere
	else if (colliderType == AABB && _object.colliderType == sphere)
	{
		float x = max(-transform.size.x * 10 + transform.position.x, min(_object.transform.position.x, transform.size.x + transform.position.x * 10));
		float y = max(-transform.size.y * 10 + transform.position.y, min(_object.transform.position.y, transform.size.y + transform.position.y * 10));
		float z = max(-transform.size.z * 10 + transform.position.z, min(_object.transform.position.z, transform.size.z + transform.position.z * 10));

		float objDistance = sqrt(pow((x - _object.transform.position.x), 2) +
			pow((y - _object.transform.position.y), 2) +
			pow((z - _object.transform.position.z), 2));


		if (objDistance < transform.size.x * 20) 
		{
			return true;
		}
		else return false;
	}
}

Object::~Object()
{
	
}
