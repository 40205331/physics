#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>
#include <vector>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"


// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
//#include "Body.h"
#include "Particle.h"


void Collision_Detect(glm::vec3 corner, glm::vec3 wall, Particle &particle)
{
	for (int i = 0; i < 3; i++)
	{
		if (particle.getPos()[i] < corner[i])
		{
			particle.setPos(i, corner[i]);
			particle.setVel(i, particle.getVel()[i] * -0.5f);
		}
		if (particle.getPos()[i] > corner[i] + wall[i])
		{
			particle.setPos(i, corner[i] + wall[i]);
			particle.setVel(i, particle.getVel()[i] * -0.5f);
		}
	}
}

// time
//GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



// main function
int main()
{
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 20.0f));
			
	// create ground plane
	Mesh plane = Mesh::Mesh();
	// scale it up x5
	plane.scale(glm::vec3(5.0f, 5.0f, 5.0f));
	plane.setShader(Shader("resources/shaders/core.vert", "resources/shaders/core.frag"));

	//declare number of particles and create list
	float numberparticles = 10;
	std::vector<Particle> list;
	//initial position for first particle
	glm::vec3 start = glm::vec3(-2.5f, 5.0f, 0.0f);
	float distance = 0.5f;
	//create for loop to create particles and add to list
	for (int i = 0; i < numberparticles; i++)
	{
		Particle particle = Particle::Particle();
		particle.getMesh().setShader(Shader("resources/shaders/core.vert", "resources/shaders/core_blue.frag"));
		particle.setMass(0.3f);
		if (i == 0)
		{
			//set velocity and position of non moving particle
			particle.setVel(glm::vec3(0.0f));
			particle.setPos(glm::vec3(start));
		}
		else if (i > 0)
		{
			//set velocity and position of moving particles
			particle.setVel(glm::vec3(0.01f));
			particle.setPos(start + glm::vec3(distance, 0.0f, 0.0f) * i);
		}
		list.push_back(particle);
	}
	
	//glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);
	Gravity g = Gravity();
	/*
	particle1.addForce(&g);
	particle3.addForce(&g);
	
	//particle1.addForce(new Drag());
	particle1.addForce(new Hook(&particle1, &particle2, 10.0f, 0.5f, 3.5f));
	particle1.addForce(new Hook(&particle1, &particle3, 10.0f, 0.5f, 3.5f));
	particle3.addForce(new Hook(&particle3, &particle1, 10.0f, 0.5f, 3.0f));
	*/
	//Apply forces to particles
	for (int i = 0; i < list.size(); i++)
	{
		/*if (i == list.size() - 1)
		{
			list[i].addForce(&g);
			list[i].addForce(new Drag());
			list[i].addForce(new Hook(&list[i], &list[i - 1], 10.0f, 0.5f, 0.75f));

		}
		*/
		if (i > 0 && i < list.size() - 1)
		{
			list[i].addForce(&g);
			list[i].addForce(new Drag());
			list[i].addForce(new Hook(&list[i], &list[i + 1], 10.0f, 0.8f, 0.5f));
			list[i].addForce(new Hook(&list[i], &list[i - 1], 10.0f, 0.8f, 0.5f));
		}
	}
	glm::vec3 o = glm::vec3(-2.5, 0.0f, 2.5f);
	glm::vec3 d = glm::vec3(5.0f, 5.0f, 5.0f);


	// time
	//GLfloat firstFrame = (GLfloat) glfwGetTime();
	
	double t = 0.0f;
	const double deltaTime = 0.001f;

	double current_time = (GLfloat)glfwGetTime();
	double accumalator = 0.0f;

	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// Set frame time
		//GLfloat currentFrame = (GLfloat)  glfwGetTime() - firstFrame;
		// the animation can be sped up or slowed down by multiplying currentFrame by a factor.
		/*
		currentFrame *= 1.5f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		*/
		
		double new_time = (GLfloat)glfwGetTime();
		double frame = new_time - current_time;
		current_time = new_time;

		accumalator += frame;

		/*
		**	INTERACTION
		*/
		// Manage interaction
		app.doMovement(deltaTime);


		/*
		**	SIMULATION
		*/
		while (accumalator >= deltaTime)
		{
			//particle1.setAcc(g);
			for (int i = 0; i < list.size(); i++)
			{
				list[i].setAcc(list[i].applyForces(list[i].getPos(), list[i].getVel(), t, deltaTime));
				list[i].setVel(list[i].getVel() + deltaTime*list[i].getAcc());
				glm::vec3 move = deltaTime*list[i].getVel();
				list[i].translate(move);


			}
			accumalator -= deltaTime;
			//t += deltaTime;
		}
		for (int i = 0; i < list.size(); i++)
		{
			Collision_Detect(o, d, list[i]);
		}
		/*
		**	RENDER 
		*/		
		// clear buffer
		app.clear();
		// draw groud plane
		app.draw(plane);
		// draw particles
		for (int i = 0; i < list.size(); i++)
		{
			app.draw(list[i].getMesh());
		}



		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

