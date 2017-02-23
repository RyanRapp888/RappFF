#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "Display.h"
#include "GameMap.h"
#include "Character.h"
#include "TheGame.h"
#include <stdio.h>
#include <direct.h>

static void PrintGLParams();
static void error_callback(int error, const char* description);
static void PrintCurrentPath();
TheGame rappff;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	rappff.KeyHandler(key, scancode, action, mods);
}

int main(void)
{
	SetKeyCallbackFunc(KeyCallback);
	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit())
	{
		std::cout << "Error initializing glfw";
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	rappff.Play();

	return 0;
}

void PrintGLParams()
{
	GLenum params[] = {
	   GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
	   GL_MAX_CUBE_MAP_TEXTURE_SIZE,
	   GL_MAX_DRAW_BUFFERS,
	   GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
	   GL_MAX_TEXTURE_IMAGE_UNITS,
	   GL_MAX_TEXTURE_SIZE,
	   GL_MAX_VARYING_FLOATS,
	   GL_MAX_VERTEX_ATTRIBS,
	   GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
	   GL_MAX_VERTEX_UNIFORM_COMPONENTS,
	   GL_MAX_VIEWPORT_DIMS,
	   GL_STEREO,
	};

	const char *names[] = {
	   "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
	   "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
	   "GL_MAX_DRAW_BUFFERS",
	   "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
	   "GL_MAX_TEXTURE_IMAGE_UNITS",
	   "GL_MAX_TEXTURE_SIZE",
	   "GL_MAX_VARYING_FLOATS",
	   "GL_MAX_VERTEX_ATTRIBS",
	   "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
	   "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
	   "GL_MAX_VIEWPORT_DIMS",
	   "GL_STEREO",
	};

	std::cout << "GL Context Params:\n";
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++)
	{
		int v = 0;
		glGetIntegerv(params[i], &v);
		std::cout << names[i] << " = " << v << std::endl;
	}

	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	printf("%s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	printf("%s %i\n", names[11], (unsigned int)s);
	printf("-----------------------------\n");
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}



void PrintCurrentPath()
{
   char cCurrentPath[FILENAME_MAX];

   if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
   {
      return;
   }

   cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; 
   printf("The current working directory is %s\n\n", cCurrentPath);
}