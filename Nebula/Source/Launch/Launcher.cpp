/**
The launcher is the entry point to the program.

@date edited 24/11/2016
@date authored 10/09/2016

@author Nathan Sainsbury */

// Must be defined before GLEW inclusion
#define GLEW_STATIC
#include <GLEW\glew.h>
#include <GLFW\glfw3.h>

#include <FreeType\ft2build.h>
#include FT_FREETYPE_H

#include <GLM\glm.hpp>

#include <map>
#include <string>
#include <iostream>

#include "Engine/EngineBuildConfig.h"
#include "Engine/EngineLimits.h"
#include "Engine/Engine.h"

#include "Engine/System/Tools/DirectoryListing.h"

int main()
{
	///////////////////////////////////////////////////////////////////////////
	// Testing area

	/////////////////////////
	// Glm
	glm::mat2 m2;
	m2.length();

	/////////////////////////
	// GLFW
	if (glfwInit() == GLFW_FALSE)
	{
		return 0;
	}
	GLFWwindow* window = glfwCreateWindow(200, 200, "Test", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	/////////////////////////
	// Freetype
	FT_Library library;
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		return 0;
	}

	/////////////////////////
	// GLEW
	glewExperimental = true;
	GLenum initResult = glewInit();
	if (initResult != GLEW_OK)
	{
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////

	#ifdef NEB_USE_LAUNCH_MENU

	std::string sInput = "";
	while (sInput != "1")
	{	
		std::cout << std::endl << "=== Nebula launch menu ===" << std::endl;
		std::cout << "1) Exit engine" << std::endl;
		std::cout << "2) Start engine" << std::endl;

		getline(std::cin, sInput);
		if (sInput == "2")
		{
			std::cout << "Starting Nebula engine..." << std::endl;

			EngineLimits limits;
			Engine engine(limits);
			if (!engine.startUp())
			{
				std::cout << "Failed start up" << std::endl;
				continue;
			}
			else
			{
				std::cout << "Done" << std::endl;
			}

			engine.run();

			std::cout << "Stopping Nebula engine..." << std::endl;
			if (!engine.shutDown())
			{
				std::cout << "Failed shut down" << std::endl;
			}
			else
			{
				std::cout << "Done" << std::endl;
			}
		}
	}

	return 0;
	
	#else 	
	
	EngineLimits limits;
	Engine engine(limits);
	if (engine.startUp())
	{
		engine.run();
		engine.shutDown();
		return 0;
	}
	else
	{
		return 1;
	}

	#endif
}