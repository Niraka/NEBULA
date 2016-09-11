/**
The launcher is the entry point to the program.

@date edited 10/09/2016
@date authored 10/09 /2016

@author Nathan Sainsbury */

// Must be defined before GLEW inclusion
#define GLEW_STATIC
#include <GLEW\glew.h>
#include <GLFW\glfw3.h>

#include <FreeType\ft2build.h>
#include FT_FREETYPE_H

#include <GLM\glm.hpp>

#include <iostream>
#include <string>

#include "Engine/EngineLimits.h"
#include "Engine/Engine.h"

int main()
{
	EngineLimits limits;
	Engine engine(limits);

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
	std::cout << "Enter 'q' to exit.." << std::endl;

	std::string sInput = "";
	while (sInput != "q")
	{
		getline(std::cin, sInput);
	}

	return 0;
}