#ifndef DISPLAY_H__
#define DISPLAY_H__

#include <string>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "Viewport.h"

void SetKeyCallbackFunc(void(*foo)(GLFWwindow *, int, int, int, int) );

class Display : public Viewport
{

public:

	Display(int width, int height, const std::string& title);
	virtual ~Display();
	void Clear(float r, float g, float b, float a);
	void SwapBuffers();
	bool WindowShouldClose();
	bool Refresh();
	GLFWwindow *GetWindowPtr();

private:

	void operator=(const Display& display) {}
	Display(const Display& display) {}
	GLFWwindow* m_window;
};

#endif
