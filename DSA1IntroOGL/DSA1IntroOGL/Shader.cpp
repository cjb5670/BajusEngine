#include "Shader.h"
#include <fstream>
#include <iostream>



Shader::Shader()
{
}


Shader::~Shader()
{
}

bool Shader::load()
{
	// return if it doesn't compile
	if (!compile(GL_VERTEX_SHADER) ||
		!compile(GL_FRAGMENT_SHADER))
	{
		return false;
	}

	// creates and links programs
	program = glCreateProgram();
	glAttachShader(program, vprogram);
	glAttachShader(program, fprogram);
	glLinkProgram(program);

	GLint IV; 
	glGetProgramiv(program, GL_LINK_STATUS, &IV);
	if (IV != 0)
	{
		// if no errors
		return true;
	}
	else
	{
		// print out errors
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* IVarray = new char[length + 1];
		IVarray[length] = 0;

		glGetProgramInfoLog(program, length, 0, IVarray);
		std::cout << IVarray;
		glDeleteProgram(program);
		delete[] IVarray;
		return false;
	}


}

bool Shader::compile(GLenum shaderType)
{
	
	char* fileContents;

	// vertex shader
	if (shaderType == GL_VERTEX_SHADER)
	{
		std::ifstream inFile(filenamev, std::ios::binary);

		if (inFile.is_open())
		{
			inFile.seekg(0, std::ios::end);
			int length = (int)inFile.tellg();
			inFile.seekg(0, std::ios::beg);

			// Move data from file to char*;
			fileContents = new char[length + 1];
			inFile.read(fileContents, length);
			fileContents[length] = 0;
			inFile.close();
		}
		else
		{
			std::cout << "ERROR: Vertex Shader file did not open correctly\n";
			return false;
		}
	}

	// fragment shader
	else if (shaderType == GL_FRAGMENT_SHADER)
	{
		std::ifstream inFile(filenamef, std::ios::binary);

		if (inFile.is_open())
		{
			inFile.seekg(0, std::ios::end);
			int length = (int)inFile.tellg();
			inFile.seekg(0, std::ios::beg);

			// Move data from file to char*;
			fileContents = new char[length + 1];
			inFile.read(fileContents, length);
			fileContents[length] = 0;
			inFile.close();
		}
		else
		{
			std::cout << "ERROR: Fragment Shader file did not open correctly\n";
			return false;
		}
	}
	else
	{
		std::cout << "ERROR: Unable to load Shader file \n";
		return false;
	}

	// Shaders loaded correctly, save them.

	GLuint index;
	index = glCreateShader(shaderType);
	if (shaderType == GL_VERTEX_SHADER)
	{
		vprogram = index;
	}
	else if (shaderType == GL_FRAGMENT_SHADER)
	{
		fprogram = index;
	}
	glShaderSource(index, 1, &fileContents, 0);
	glCompileShader(index);
	delete[] fileContents;

	GLint IV; 
	glGetShaderiv(index, GL_COMPILE_STATUS, &IV);
	if (IV != 0)
	{
		return true;
	}
	else
	{
		GLint length;
		glGetShaderiv(index, GL_INFO_LOG_LENGTH, &length);
		char* IVarray = new char[length + 1];
		IVarray[length] = 0;

		glGetShaderInfoLog(index, length, 0, IVarray);
		std::cout << IVarray;
		glDeleteShader(index);
		delete[] IVarray;
		return false;
	}
	
	
}

void Shader::use()
{
	glUseProgram(program);
}

void Shader::unload()
{
	glDeleteProgram(program);
	glDeleteShader(vprogram);
	glDeleteShader(fprogram);
	program = 0;
	vprogram = 0;
	fprogram = 0;
}
