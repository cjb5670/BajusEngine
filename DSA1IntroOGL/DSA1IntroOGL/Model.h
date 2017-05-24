#pragma once
#include <FreeImage.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace glm;

struct Vertex 
{
	vec3 location;
	vec2 uv;
	vec3 normal;
};

struct VertInd
{
	unsigned int locIndex;
	unsigned int uvIndex;
	unsigned int normIndex;
};
class Model
{
private:
	unsigned int vertCount;
	GLuint vertArr;
	GLuint vertBuf;
public:
	Model();
	~Model();

	bool buffer(std::string objFile);
	void render();
	float xMax = 0;
	float yMax = 0;
	float zMax = 0;

	float xMin = 9999999;
	float yMin = 9999999;
	float zMin = 9999999;

	vec3 objColor = vec3(1, 1, 1);
};

