
#include <glad.h>
#include "TiledGameBoard.h"

#include "GameMap.h"
#include "Character.h"
#include "Display.h"

class Sandbox
{
public:
	Sandbox() {}
	void KeyHandler(int key, int scancode, int action, int mods);
	void Play();
	~Sandbox()
	{
		//clean all the stuff
	}

private:
	void Interact(int x, int y);
	Character m_mainchar;
	Display *m_display_ptr;
	GameMap *m_gamemap_ptr;
};

void Sandbox::KeyHandler(int key, int scancode, int action, int mods)
{
	int abc = 0;
}

void Sandbox::Play()
{
	m_display_ptr = new Display(800, 600, "Sandbox");
	if (m_display_ptr == nullptr) return;

	/*
	Tile *lower = new Tile();
	lower->SetTileType(TileType::MTNSNOW);
	lower->SetRelativeLocation(.01,.01,.98,.98);
	lower->SetColor(RGB(255,255,255));
	m_display_ptr->AddObject(lower);
	*/

	while (!m_display_ptr->WindowShouldClose())
	{
		m_display_ptr->Clear(0, 0, .2, 1);
		m_display_ptr->Refresh();
		m_display_ptr->SwapBuffers();
		glfwPollEvents();
	}
}

std::string ToText(ProxRel dat)
{
	if (dat == ProxRel::FOUND_TO_LEFT) return "on your left!";
	else if (dat == ProxRel::FOUND_TO_RIGHT) return "on your right!";
	else if (dat == ProxRel::FOUND_ABOVE) return "to the NORTH of you!";
	else if (dat == ProxRel::FOUND_BELOW) return "to the SOUTH of you!";
	return "";
}

void Sandbox::Interact(int x, int y)
{
	int aa = 0;
}

Sandbox abc;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	abc.Play();
		
    return 0;
}

