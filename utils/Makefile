CXX = cl.exe
CXXFLAGS = /EHsc /c /Zi -MD

all: Observer.lib glad.lib WinViewportUtils.lib

Observer.lib: 
	$(CXX) $(CXXFLAGS) Observer.cpp
	lib Observer.obj
	mv Observer.lib ../lib

glad.lib:
	$(CXX) $(CXXFLAGS) glad.c
	lib glad.obj
	mv glad.lib ../lib

WinViewportUtils.lib:
	$(CXX) $(CXXFLAGS) WinViewportUtils.cpp
	lib WinViewportUtils.obj
	mv WinViewportUtils.lib ../lib
