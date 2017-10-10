#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

// Std. Includes
#include <string>
#include <time.h>

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
	
		if (particle.getPos()[1] < corner[1])
		{
			particle.setPos(1, corner[1]);
			particle.setVel(1, particle.getVel()[1] * -0.9f);
		}
		if (particle.getPos()[1] > corner[1] + wall[1])
		{
			particle.setPos(1, corner[1] + wall[1]);
			particle.setVel(1, particle.getVel()[1] * -0.9f);
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


	// create particle
	Particle particle1 = Particle::Particle();
	particle1.getMesh().setShader(Shader("resources/shaders/core.vert", "resources/shaders/core_blue.frag"));
	Particle particle2 = Particle::Particle();
	particle2.getMesh().setShader(Shader("resources/shaders/core.vert", "resources/shaders/core_blue.frag"));
	Particle particle3 = Particle::Particle();
	particle3.getMesh().setShader(Shader("resources/shaders/core.vert", "resources/shaders/core_blue.frag"));


	//Add vectors for particle movement
	particle1.setPos(glm::vec3(0.0f, 1.0f, 0.0f));
	particle1.setVel(glm::vec3(0.0f, 0.0f, 0.0f));

	particle2.setPos(glm::vec3(0.0f, 5.0f, 0.0f));
	particle2.setVel(glm::vec3(0.0f, 0.0f, 0.0f));

	particle3.setPos(glm::vec3(0.0f, 2.0f, 0.0f));
	particle3.setVel(glm::vec3(0.0f, 0.0f, 0.0f));

	//glm::vec3 g = glm::vec3(0.0f, -9.8f, 0.0f);
	Gravity g = Gravity(glm::vec3(0.0f, -9.8f, 0.0f));
	particle1.addForce(&g);
	particle3.addForce(&g);
	
	//particle1.addForce(new Drag());
	particle1.addForce(new Hook(&particle1, &particle2, 10.0f, 0.5f, 3.5f));
	particle1.addForce(new Hook(&particle1, &particle3, 10.0f, 0.5f, 3.5f));
	particle3.addForce(new Hook(&particle3, &particle1, 10.0f, 0.5f, 3.0f));

	glm::vec3 o = glm::vec3(-2.5, 0.0f, 2.5f);
	glm::vec3 d = glm::vec3(5.0f, 5.0f, 5.0f);


	// time
	//GLfloat firstFrame = (GLfloat) glfwGetTime();
	
	double t = 0.0f;
	const double deltaTime = 0.01f;

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
			particle1.setAcc(particle1.applyForces(particle1.getPos(), particle1.getVel(), t, deltaTime));
			particle1.setVel(particle1.getVel() + deltaTime*particle1.getAcc());
			glm::vec3 move = deltaTime*particle1.getVel();
			particle1.translate(move);

			particle3.setAcc(particle3.applyForces(particle3.getPos(), particle3.getVel(), t, deltaTime));
			particle3.setVel(particle3.getVel() + deltaTime*particle3.getAcc());
			glm::vec3 move3 = deltaTime*particle3.getVel();
			particle1.translate(move3);
			

			accumalator -= deltaTime;
			//t += deltaTime;
		}
		
		Collision_Detect(o, d, particle1);
		Collision_Detect(o, d, particle3);

		/*
		**	RENDER 
		*/		
		// clear buffer
		app.clear();
		// draw groud plane
		app.draw(plane);
		// draw particles
		app.draw(particle1.getMesh());
		app.draw(particle2.getMesh());
		app.draw(particle3.getMesh());


		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

