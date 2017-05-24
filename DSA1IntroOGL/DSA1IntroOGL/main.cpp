#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include <time.h>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
//#include "Model.h"
#include "Object.h"
using namespace glm;

float randomloc()
{
	return ((rand() % 40) - 20) / 10;
}
//test

float randomRot()
{
	return (rand() % 60) / 10;
}

int main()
{
	if (glfwInit() == GL_FALSE)
		return -1;

	GLFWwindow* GLFWwindowPtr =
		glfwCreateWindow(800, 600, "Cameron DSA1 Engine", NULL, NULL);

	if (GLFWwindowPtr != nullptr)
		glfwMakeContextCurrent(GLFWwindowPtr);
	else
	{
		glfwTerminate();
		return -1;
	}

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	// Sets background color
	glClearColor(0, 0, 0, 1.0f);





	// Loading objects.
	// Enemies
	const unsigned int wfSize = 3;						// CHANGE WITH SIZE OF OBJECTS
	std::vector<Object> WideFighterList(wfSize);
	const char* wfighter = "models/widefighter.obj";
	const char* wfightertex = "textures/metalLight.png";
	for (int i = 0; i < wfSize; i++)
	{
		WideFighterList[i] = Object(wfightertex, wfighter, GLFWwindowPtr, Object::AABB); // HERE IS WHERE YOU SET THE TYPE OF COLLISION BOX FOR NON PLAYER
		WideFighterList[i].transform.SetForces(1, 1);
	}
	// Random Generation.
	srand((unsigned int)time(0));

	// Right 
	WideFighterList[0].setTransforms(vec3(randomloc(), randomloc(), 0), vec3(-1.57, 0, 0), vec3(.001, .001, .001));
	// Middle 
	 WideFighterList[1].setTransforms(vec3(randomloc(), randomloc(), 0), vec3(-1.57, 0, 0), vec3(.001, .001, .001));
	//// Left
	 WideFighterList[2].setTransforms(vec3(randomloc(), randomloc(), 0), vec3(-1.57, 0, 0), vec3(.001, .001, .001));
	//// Right 



	// Friendly
	const unsigned int sfSize = 1;						// CHANGE WITH SIZE OF OBJECTS
	std::vector<Object> SleekFighterList(sfSize);
	const char* sfighter = "models/sleekfighter.obj";
	const char* sfightertex = "textures/metalLight.png";
	for (int i = 0; i < sfSize; i++)
	{
		SleekFighterList[i] = Object(sfightertex, sfighter, GLFWwindowPtr, Object::AABB);  // HERE IS WHERE YOU SET THE TYPE OF COLLISION BOX FOR PLAYER
		SleekFighterList[i].transform.SetForces(.5, 2);
		//SleekFighterList[i].transform.SetWander(.001, .001, .00000001);
	}
	// Player
	SleekFighterList[0].setTransforms(vec3(0, -.075, 1.65), vec3(-1.57, 0, 0), vec3(.01, .01, .01));

	// Missile
	const unsigned int mSize = 1;						// CHANGE WITH SIZE OF OBJECTS
	std::vector<Object> MissileList = std::vector<Object>();
	const char* mObj = "models/sphere.obj";
	const char* misTex = "textures/blue.png";
		


	
	Object * playerPtr = &SleekFighterList[0];

	// Loading camera
	Camera camera = Camera(*playerPtr);

	
	// timer varibles
	float t = 0;
	float t0;
	float dt;

	bool hasWon = false;

	while (!glfwWindowShouldClose(GLFWwindowPtr) && !(glfwGetKey(GLFWwindowPtr, GLFW_KEY_ESCAPE)) && !hasWon)
	{
		// calculate time
		t0 = t;
		t = (float)glfwGetTime();	
		dt = t - t0;
		glUniform1f(1, t);
		//upload camera matrix

		camera.Update();

		// draw models
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Render 3d model
		
		// calls update of each object
		for (int i = 0; i < wfSize; i++)
		{
			
			if (!WideFighterList[i].isDisabled)
			{
				WideFighterList[i].transform.updateEnemy(dt);				
			}
			WideFighterList[i].update();
		}
		
		for (int i = 0; i < sfSize; i++)
		{
			SleekFighterList[i].transform.updateFriend(dt);
			SleekFighterList[i].update();
		}


		// shoots projectile
		// if no projectiles
		if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) && MissileList.size() == 0)
		{
			// Spawns missile
			MissileList.push_back(Object(misTex, mObj, GLFWwindowPtr, Object::AABB));
			MissileList[0].transform.SetForces(.01, 5);
			// sets it below player 
			MissileList[0].setTransforms(vec3(SleekFighterList[0].transform.location.x, 
				SleekFighterList[0].transform.location.y, 
				SleekFighterList[0].transform.location.z), vec3(0, 0, 0), vec3(.1, .1, .1));
			// Makes missile go forward
			MissileList[0].transform.ApplyForce(SleekFighterList[0].transform.direction * vec3(10, 0, 10));
		}

		if (MissileList.size() > 0)
		{
			MissileList[0].transform.updateMissile(dt);
			MissileList[0].update();
			if (MissileList[0].transform.MissileDecay(dt, 2))
			{
				MissileList[0].unload();
				MissileList.pop_back();
			}
		}

		// collisions
		for (int i = 0; i < wfSize; i++)
		{
			// Captures ships
			if (SleekFighterList[0].collidesWith(WideFighterList[i]) && WideFighterList[i].isDisabled)
			{
				WideFighterList[i].isCaptured = true;
				WideFighterList[i].model.objColor = vec3(0, 1, 0);
			}
			// Disables ships
			if (MissileList.size() > 0)
			{
				if (MissileList[0].collidesWith(WideFighterList[i]))
				{
					WideFighterList[i].isDisabled = true;
				}
			}

		}

		for (int i = 0; i < wfSize; i++)
		{
			if (WideFighterList[i].isCaptured == false)
			{
				break;
			}
			if (i + 1 == wfSize)
			{
				hasWon = true;
			}
		}
		


		
		glfwSwapBuffers(GLFWwindowPtr);

		glfwPollEvents();
	}
	
	for (int i = 0; i < wfSize; i++)
	{
		WideFighterList[i].unload();
	}

	for (int i = 0; i < sfSize; i++)
	{
		SleekFighterList[i].unload();
	}
	
	glfwTerminate();
	return 0;
}