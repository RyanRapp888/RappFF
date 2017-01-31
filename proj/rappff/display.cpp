#include "display.h"
#include <iostream>

int cd_width, cd_height;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
   }
}

static void WindowSizeCallback(GLFWwindow *window, int w, int h)
{
   cd_width = w;
   cd_height = h;
   glViewport(0,0,w,h);
}

static void WindowCloseCallback(GLFWwindow *window)
{
   glfwSetWindowShouldClose(window, GLFW_TRUE);
}

Display::Display(int width, int height, const std::string& title)
{
   cd_width = width;
   cd_height = height; 
   glfwWindowHint(GLFW_RESIZABLE,GLFW_TRUE); 
   m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
   glfwSetWindowSizeCallback(m_window, WindowSizeCallback);
   glfwSetWindowCloseCallback(m_window, WindowCloseCallback);
   glfwSetKeyCallback(m_window, KeyCallback);
   glfwMakeContextCurrent(m_window);
   gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
   glfwSwapInterval(1);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

bool Display::Refresh()
{
   static int prev_ww = 0;
   static int prev_hh = 0;

   if(prev_ww != cd_width || prev_hh != cd_height)
   {
      prev_ww = cd_width;
      prev_hh = cd_height;
      Viewport::SetWindowSize(cd_width, cd_height);
   }

   Viewport::Refresh();

   return true;
}

Display::~Display()
{
   glfwDestroyWindow(m_window);
   m_window = nullptr;
}

void Display::Clear(float r, float g, float b, float a)
{
   int width,height;
   glfwGetFramebufferSize(m_window, &width, &height);
   glfwGetWindowSize(m_window, &width, &height);
   glClearColor(r, g, b, a);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::SwapBuffers()
{
   glfwSwapBuffers(m_window);
}

bool Display::WindowShouldClose()
{
   return glfwWindowShouldClose(m_window);
}