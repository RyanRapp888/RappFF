
#include <glad.h>
#include "TheGame.h"
#include "TiledGameBoard.h"
#include "Shader.h"
#include "Camera.h"

int get_rand()
{
	static bool is_seeded(false);
	if (!is_seeded)
	{
		srand(14);
		is_seeded = true;
	}
	int result = rand() % 100;
	return result;
}

void TheGame::KeyHandler(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		int xx = m_mainchar.GetX();
		int yy = m_mainchar.GetY();
		bool took_step(false);
		switch (key)
		{
			case(GLFW_KEY_W):
			case(GLFW_KEY_UP):
				yy++;
				took_step = true;
				break;
			case(GLFW_KEY_A):
			case(GLFW_KEY_LEFT):
				xx++;
				took_step = true;
				break;
			case(GLFW_KEY_S):
			case(GLFW_KEY_DOWN):
				yy--;
				took_step = true;
				break;
			case(GLFW_KEY_D):
			case(GLFW_KEY_RIGHT):
				xx--;
				took_step = true;
				break;
			case(GLFW_KEY_X):
				m_xrot+=5;
				break;
			case(GLFW_KEY_Y):
				m_yrot+=5;
				break;
			case(GLFW_KEY_Z):
				m_zrot+=5;
			case(GLFW_KEY_ENTER):
				Interact(m_mainchar.GetX(), m_mainchar.GetY());
			default:
				break;
		}
		
		if (took_step)
		{
			m_mainchar.SetLocation(xx, yy);
			GameMap *gamemap_ptr = GameMap::GetInstance();
			int randy = get_rand();
			int monster_odds = gamemap_ptr->GetMonsterOdds(xx,yy);
			if (randy <= monster_odds)
			{
				std::cout << "FIGHTY MODE!!!" << std::endl;
				m_fightymode = true;
			}
			std::cout << randy << " out of " << monster_odds << std::endl;
			std::cout << "(" << xx << "," << yy << ")" << std::endl;
		}
		
	}
}

void TheGame::Play()
{
	m_display_ptr = new Display(800, 600, "Cupcake");
	if (m_display_ptr == nullptr) return;
	
	//ultimately, I want gamemap to be 256x256
	GameMap *gamemap_ptr = GameMap::GetInstance();
	
	if (!gamemap_ptr->LoadGameMap(256, 256))
	{
		std::cout << "Error: Could not load game map" << std::endl;
	}

	m_mainchar.SetName("main_character");
	m_mainchar.SetCharacterType(CharacterType::MAINCHAR);
	m_mainchar.SetLocation(128, 05);
	gamemap_ptr->AttachMainCharacter(&m_mainchar);
	TiledGameBoard *tiled_ws = new TiledGameBoard(m_display_ptr, .01, .01, .98, .98);
	Shader InstancedShader("res\\basicShader");
	Shader NonInstancedShader("res\\basic_noninst");
	tiled_ws->SetTileDetails(20, 20);
	m_display_ptr->AddWindowSection(tiled_ws);
	
	Tile *singular = new Tile();
	singular->SetTileType(TileType::PARCHMENT);
	singular->SetRelativeLocation(0, 0, 1, 1);
	WindowSection *fightmode_bl = new WindowSection(m_display_ptr, 0, 0, .357, .35);
	fightmode_bl->AddObject(singular);
	WindowSection *fightmode_tl = new WindowSection(m_display_ptr, 0, .357, .535, .65);
	fightmode_tl->AddObject(singular);
	WindowSection *fightmode_br = new WindowSection(m_display_ptr, .357, 0, .428, .35);
	fightmode_br->AddObject(singular);
	WindowSection *fightmode_tr = new WindowSection(m_display_ptr, .535, .35, .264, .65);
	fightmode_tr->AddObject(singular);
	WindowSection *fightmode_fr = new WindowSection(m_display_ptr, .785, 0, .214, 1);
	fightmode_fr->AddObject(singular);
	fightmode_bl->Disable();
	fightmode_tl->Disable();
	fightmode_br->Disable();
	fightmode_tr->Disable();
	fightmode_fr->Disable();
	m_display_ptr->AddWindowSection(fightmode_bl);
	m_display_ptr->AddWindowSection(fightmode_tl);
	m_display_ptr->AddWindowSection(fightmode_br);
	m_display_ptr->AddWindowSection(fightmode_tr);
	m_display_ptr->AddWindowSection(fightmode_fr);

	std::vector<Character> otherchars;
	otherchars.resize(5);
	otherchars[0].SetName("Emmy");
	otherchars[0].SetCharacterType(CharacterType::EMMY);
	otherchars[0].SetLocation( 120, 38);
	otherchars[1].SetName("Lily");
	otherchars[1].SetCharacterType(CharacterType::FAIRY);
	otherchars[1].SetLocation( 115, 33);
	otherchars[2].SetName("Jerry");
	otherchars[2].SetCharacterType(CharacterType::OCTOPUS);
	otherchars[2].SetLocation(133, 9);
	otherchars[3].SetName("Bella");
	otherchars[3].SetCharacterType(CharacterType::JELLYBEAN);
	otherchars[3].SetLocation(120, 45);
	otherchars[4].SetName("Maddy");
	otherchars[4].SetCharacterType(CharacterType::MERMAID);
	otherchars[4].SetLocation(128, 20);
	
	for (int bb = 0; bb < otherchars.size(); bb++)
	{
		gamemap_ptr->AttachCharacter(&(otherchars[bb]));
	}
	
	Transform maintransform;
	Transform fighttransform;
	Camera maincamera(glm::vec3(0, 0, -1.2), 66.0f, 800 / 600, 0.1f, 100.0f);
	Camera fightcamera(glm::vec3(0, 0, -1.1), 90.0f, 800 / 600, 0.1f, 100.0f);
	m_xrot = 40;
	m_yrot = 0;
	m_zrot = 0;
	RGB background(0,0,0);

	while (!m_display_ptr->WindowShouldClose())
	{
		m_display_ptr->Clear(
			background.GetRed()/255.0,
			background.GetGreen()/255.0,
			background.GetBlue()/255.0,
			1);

		if (m_fightymode)
		{
			InstancedShader.Detach();
			NonInstancedShader.Bind();
			tiled_ws->Disable();
			fightmode_bl->Enable();
			fightmode_tl->Enable();
			fightmode_br->Enable();
			fightmode_tr->Enable();
			fightmode_fr->Enable();

			if (!m_fightymode)
			{
     			tiled_ws->Enable();
				fightmode_bl->Disable();
				fightmode_tl->Disable();
				fightmode_br->Disable();
				fightmode_tr->Disable();
				fightmode_fr->Disable();
			}
			NonInstancedShader.Update(fighttransform, fightcamera);
		}
		else
		{
			NonInstancedShader.Detach();
			InstancedShader.Bind();
			maintransform.GetRot()->x = m_xrot;
			maintransform.GetRot()->y = m_yrot;
			maintransform.GetRot()->z = m_zrot;
			InstancedShader.Update(maintransform, maincamera);
		}
		m_display_ptr->Refresh();
		m_display_ptr->SwapBuffers();
		glfwPollEvents();
	}
}

std::string ToText(ProxRel dat)
{
	if (dat == ProxRel::FOUND_TO_LEFT) return "to the EAST of you!";
	else if (dat == ProxRel::FOUND_TO_RIGHT) return "to the RIGHT of you!";
	else if (dat == ProxRel::FOUND_ABOVE) return "to the NORTH of you!";
	else if (dat == ProxRel::FOUND_BELOW) return "to the SOUTH of you!";
	return "";
}

void TheGame::Interact(int x, int y)
{
	std::vector<Character *> foundchars;
	std::vector< ProxRel > proximities;

	GameMap *map_ptr = GameMap::GetInstance();
	map_ptr->FindTouchingCharacters(
		m_mainchar.GetX(),
		m_mainchar.GetY(),
		foundchars, proximities);

	for (int aa = 0; aa < foundchars.size(); aa++)
	{
		std::cout << "YOU FOUND " << foundchars[aa]->GetName() << "!!!" << std::endl;
		std::cout << foundchars[aa]->GetName() << " is " << ToText(proximities[aa]) << std::endl
			      << "!! How FUN!!" << std::endl;
	}
}