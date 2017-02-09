
#include <glad.h>
#include "TheGame.h"
#include "TiledGameBoard.h"
#include "Shader.h"
#include "Camera.h"

void TheGame::KeyHandler(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		int xx = m_mainchar.GetX();
		int yy = m_mainchar.GetY();

		switch (key)
		{
			case(GLFW_KEY_W):
			case(GLFW_KEY_UP):
				yy++;
				break;
			case(GLFW_KEY_A):
			case(GLFW_KEY_LEFT):
				xx++;
				break;
			case(GLFW_KEY_S):
			case(GLFW_KEY_DOWN):
				yy--;
				break;
			case(GLFW_KEY_D):
			case(GLFW_KEY_RIGHT):
				xx--;
				break;
			case(GLFW_KEY_X):
				m_xrot++;
				break;
			case(GLFW_KEY_Y):
				m_yrot++;
				break;
			case(GLFW_KEY_Z):
				m_zrot++;
			case(GLFW_KEY_ENTER):
				Interact(m_mainchar.GetX(), m_mainchar.GetY());
			default:
				break;
		}
		m_mainchar.SetLocation(xx, yy);
	}
}

void TheGame::Play()
{
	m_display_ptr = new Display(800, 600, "Cupcake");
	if (m_display_ptr == nullptr) return;
	
	//ultimately, I want gamemap to be 256x256
	GameMap *gamemap_ptr = GameMap::GetInstance();
	if (!gamemap_ptr->LoadGameMap(40, 30))
	{
		std::cout << "Error: Could not load game map" << std::endl;
	}
	m_mainchar.SetName("main_character");
	m_mainchar.SetCharacterType(CharacterType::MAINCHAR);
	m_mainchar.SetLocation(15, 15);
	gamemap_ptr->AttachMainCharacter(&m_mainchar);
	TiledGameBoard *upper = new TiledGameBoard(m_display_ptr, .01, .01, .98, .98);
	Shader testShader("res\\basicShader");
	upper->SetTileDetails(20, 20);
	m_display_ptr->AddWindowSection(upper);
	
	std::vector<Character> otherchars;
	otherchars.resize(5);
	otherchars[0].SetName("Emmy");
	otherchars[0].SetCharacterType(CharacterType::EMMY);
	otherchars[0].SetLocation( 36, 4);
	otherchars[1].SetName("Lily");
	otherchars[1].SetCharacterType(CharacterType::FAIRY);
	otherchars[1].SetLocation( 26, 28);
	otherchars[2].SetName("Jerry");
	otherchars[2].SetCharacterType(CharacterType::OCTOPUS);
	otherchars[2].SetLocation(11, 5);
	otherchars[3].SetName("Bella");
	otherchars[3].SetCharacterType(CharacterType::JELLYBEAN);
	otherchars[3].SetLocation(2, 28);
	otherchars[4].SetName("Maddy");
	otherchars[4].SetCharacterType(CharacterType::MERMAID);
	otherchars[4].SetLocation(22, 17);
	
	for (int bb = 0; bb < otherchars.size(); bb++)
	{
		gamemap_ptr->AttachCharacter(&(otherchars[bb]));
	}
	
	testShader.Bind();
	Transform transform;
	Camera camera(glm::vec3(0, 0, -1.2f), 50.0f, 800 / 600, 0.1f, 100.0f);
	m_xrot = 20;
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
		testShader.Bind();
		transform.GetRot()->x = m_xrot;
		transform.GetRot()->y = m_yrot;
		transform.GetRot()->z = m_zrot;
		testShader.Update(transform, camera);
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