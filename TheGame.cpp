
#include <glad.h>
#include "TheGame.h"
#include "TiledGameBoard.h"

/*
#include <time.h>
static time_t prev_mvmt_key(-1);
time_t cur;
time(&cur);
if (prev_mvmt_key > 0 && fabs(difftime(prev_mvmt_key, cur)) < KEYDELAY)
{
	return;
}
prev_mvmt_key = cur;
*/

void TheGame::KeyHandler(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && (key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D ||
		key == GLFW_KEY_UP || key == GLFW_KEY_LEFT || key == GLFW_KEY_DOWN || key == GLFW_KEY_RIGHT) )
	{
		
		
		int xx = m_mainchar_ptr->GetWorldX();
		int yy = m_mainchar_ptr->GetWorldY();
		
		switch (key)
		{
		case(GLFW_KEY_W):
		case(GLFW_KEY_UP):
			yy++;
			break;
		case(GLFW_KEY_A):
		case(GLFW_KEY_LEFT):
			xx--;
			break;
		case(GLFW_KEY_S):
		case(GLFW_KEY_DOWN):
			yy--;
			break;
		case(GLFW_KEY_D):
		case(GLFW_KEY_RIGHT):
			xx++;
			break;
		default:
			break;
		}
		m_mainchar_ptr->SetLocation(xx, yy);
	}
}

void TheGame::Play()
{
	m_display_ptr = new Display(800, 600, "Cupcake");
	if (m_display_ptr == nullptr) return;
	
	/*
	Tile *lower = new Tile();
	lower->SetTileType(TileType::PARCHMENT);
	lower->SetRelativeLocation(.01,.01,.98,.28);
	lower->SetColor(RGB(255,255,255));
	display.AddObject(lower);
	*/
	//ultimately, I want gamemap to be 256x256
	m_gamemap_ptr = new GameMap(40, 30);
	TiledGameBoard *upper = new TiledGameBoard(m_display_ptr, .01, .01, .98, .98, m_gamemap_ptr);
	upper->SetTileDetails(16, 16);
	m_display_ptr->AddWindowSection(upper);
	m_mainchar_ptr = new Character("main_character", CharacterType::MAINCHAR, CharMotion(),
		22, 0, m_gamemap_ptr);
	m_mainchar_ptr->SetColor(RGB(255, 255, 255));
	upper->AttachMainCharacter(m_mainchar_ptr);

	int n_chars(5);
	Character *otherchars = new Character[n_chars];
	otherchars[0] = Character("Emme", CharacterType::EMMY, CharMotion(), 36, 4, m_gamemap_ptr);
	otherchars[1] = Character("Lily", CharacterType::FAIRY, CharMotion(), 26, 28, m_gamemap_ptr);
	//otherchars.Character("Sally",TileType::QUEEN,CharMotion(),6,6,m_gamemap_ptr);
	otherchars[2] = Character("Jerry", CharacterType::OCTOPUS, CharMotion(), 11, 5, m_gamemap_ptr);
	otherchars[3] = Character("Bella", CharacterType::JELLYBEAN, CharMotion(), 2, 28, m_gamemap_ptr);
	otherchars[4] = Character("Maddy", CharacterType::MERMAID, CharMotion(), 22, 17, m_gamemap_ptr);

	for (int bb = 0; bb < n_chars; bb++)
	{
		upper->AttachCharacter(&(otherchars[bb]));
	}

	while (!m_display_ptr->WindowShouldClose())
	{
		m_display_ptr->Clear(0, 0, .2, 1);
		m_display_ptr->Refresh();
		m_display_ptr->SwapBuffers();
		glfwPollEvents();
	}
}
