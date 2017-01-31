# example: example.cpp
#	cl -MD example.cpp glad.lib glfw3.lib User32.lib Gdi32.lib Shell32.lib

CXX =cl.exe
CXXFLAGS = /EHsc /Zi -MD /NODEFAULTLIB:library
LDLIBS = glad.lib glfw3.lib User32.lib Gdi32.lib Shell32.lib Observer.lib

all: utils example

utils:
	$(MAKE) -C ../../utils

clean:
	rm *.obj
	rm *.exe
