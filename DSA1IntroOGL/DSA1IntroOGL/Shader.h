#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <string>

class Shader
{
public:

	GLuint program, vprogram, fprogram;

	std::string filenamev = "shaders/vshader.glsl";
	std::string filenamef = "shaders/fshader.glsl";
	Shader();
	~Shader();
	bool load();
	bool compile(GLenum shaderType);
	void use();
	void unload();
};