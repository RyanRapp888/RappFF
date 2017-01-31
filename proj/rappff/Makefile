CXX =cl.exe
CXXFLAGS = /EHsc /Zi -MD
# /NODEFAULTLIB:library
LDLIBS = glad.lib glfw3.lib User32.lib Gdi32.lib Shell32.lib Observer.lib WinViewportUtils.lib

all: utils rappff


utils:
	$(MAKE) -C ../../utils

rappff:
	$(CXX) $(CXXFLAGS) rappff.cpp Display.cpp Mesh.cpp Shader.cpp Texture.cpp obj_loader.cpp stb_image.c $(LDLIBS)

clean:
	rm *.obj
	rm *.exe
