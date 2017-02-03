
#include <glad.h>
#include "TheGame.h"
#include "TiledGameBoard.h"

void TheGame::KeyHandler(int key, int scancode, int action, int mods)
{
	if (key == 263) //left arrow
	{
		int xx = m_mainchar_ptr->GetWorldX();
		int yy = m_mainchar_ptr->GetWorldY();
		m_mainchar_ptr->SetLocation(xx - 1, yy);
	}
	else if (key == 262)
	{
		int xx = m_mainchar_ptr->GetWorldX();
		int yy = m_mainchar_ptr->GetWorldY();
		m_mainchar_ptr->SetLocation(xx + 1, yy);
	}
	else if (key == 265)
	{
		int xx = m_mainchar_ptr->GetWorldX();
		int yy = m_mainchar_ptr->GetWorldY();
		m_mainchar_ptr->SetLocation(xx, yy + 1);
	}
	else if (key == 264)
	{
		int xx = m_mainchar_ptr->GetWorldX();
		int yy = m_mainchar_ptr->GetWorldY();
		m_mainchar_ptr->SetLocation(xx, yy - 1);
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
	m_mainchar_ptr = new Character("main_character", TileType::MAINCHAR, CharMotion(),
		22, 0, m_gamemap_ptr);
	m_mainchar_ptr->SetColor(RGB(255, 255, 255));
	upper->AttachMainCharacter(m_mainchar_ptr);

	int n_chars(5);
	Character *otherchars = new Character[n_chars];
	otherchars[0] = Character("Emme", TileType::EMMY, CharMotion(), 36, 4, m_gamemap_ptr);
	otherchars[1] = Character("Lily", TileType::FAIRY, CharMotion(), 26, 28, m_gamemap_ptr);
	//otherchars.Character("Sally",TileType::QUEEN,CharMotion(),6,6,m_gamemap_ptr);
	otherchars[2] = Character("Jerry", TileType::OCTOPUS, CharMotion(), 11, 5, m_gamemap_ptr);
	otherchars[3] = Character("Bella", TileType::JELLYBEAN, CharMotion(), 2, 28, m_gamemap_ptr);
	otherchars[4] = Character("Maddy", TileType::MERMAID, CharMotion(), 22, 17, m_gamemap_ptr);

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
