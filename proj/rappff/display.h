#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include <string>
//#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <utils/WinViewportUtils.h>

class TheGame;

void SetEvilPtr(TheGame *obj);

class Display : public Viewport
{
	
public:
   Display(int width, int height, const std::string& title);

   void Clear(float r, float g, float b, float a);
   void SwapBuffers();
   bool WindowShouldClose();
   bool Refresh();
   GLFWwindow *GetWindowPtr();
	
   virtual ~Display();
protected:
private:
	void operator=(const Display& display) {}
	Display(const Display& display) {}
	GLFWwindow* m_window;
};

#endif
