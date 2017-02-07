
#include <glad.h>
#include "TheGame.h"
#include "TiledGameBoard.h"
#include "Shader.h"
#include "Camera.h"

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
	m_gamemap_ptr =  new GameMap(40, 30, &m_mainchar);
	m_mainchar = Character("main_character", CharacterType::MAINCHAR, CharMotion(), 22, 4);
	m_mainchar.SetColor(RGB(255, 255, 255));
	TiledGameBoard *upper = new TiledGameBoard(m_display_ptr, .01, .01, .98, .98, m_gamemap_ptr);
	Shader testShader("res\\InstanceShader");
	upper->SetTileDetails(16, 16);
	m_display_ptr->AddWindowSection(upper);
	
	int n_chars(5);
	Character *otherchars = new Character[n_chars];
	otherchars[0] = Character("Emme", CharacterType::EMMY, CharMotion(), 36, 4);
	otherchars[1] = Character("Lily", CharacterType::FAIRY, CharMotion(), 26, 28);
	otherchars[2] = Character("Jerry", CharacterType::OCTOPUS, CharMotion(), 11, 5);
	otherchars[3] = Character("Bella", CharacterType::JELLYBEAN, CharMotion(), 2, 28);
	otherchars[4] = Character("Maddy", CharacterType::MERMAID, CharMotion(), 22, 17);

	for (int bb = 0; bb < n_chars; bb++)
	{
		m_gamemap_ptr->AttachCharacter(&(otherchars[bb]));
	}
	
	/*
	Tile tmptile;
	tmptile.SetColor(RGB(255, 255, 255));
	tmptile.SetTileType(TileType::MTN);
	m_display_ptr->AddObject(&tmptile);
	*/
	Shader testShader("res\\basicShader");
	
	testShader.Bind();
	Transform transform;
	Camera camera(glm::vec3(0.0f, 0.0f, -1.5f), 70.0f, 800 / 600, 0.1f, 100.0f);

	RGB background(255, 182, 193);

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
		std::cout << "xyz rot" << m_xrot << "," << m_yrot << "," << m_zrot << std::endl;
		testShader.Update(transform, camera);
		m_display_ptr->Refresh();
		m_display_ptr->SwapBuffers();
		glfwPollEvents();
	}
}

std::string ToText(ProxRel dat)
{
	if (dat == ProxRel::FOUND_TO_LEFT) return "to the WEST of you!";
	else if (dat == ProxRel::FOUND_TO_RIGHT) return "to the EAST of you!";
	else if (dat == ProxRel::FOUND_ABOVE) return "to the NORTH of you!";
	else if (dat == ProxRel::FOUND_BELOW) return "to the SOUTH of you!";
	return "";
}

void TheGame::Interact(int x, int y)
{
	std::vector<Character *> foundchars;
	std::vector< ProxRel > proximities;

	m_gamemap_ptr->FindTouchingCharacters(
		m_mainchar.GetX(),
		m_mainchar.GetY(),
		foundchars, proximities);

	for (int aa = 0; aa < foundchars.size(); aa++)
	{
		std::cout << "YOU FOUND " << foundchars[aa]->GetName() << "!!!" << std::endl;
		std::cout << foundchars[aa]->GetName() << " is " << ToText(proximities[aa]) << std::endl
			      << "!! How FUN!!" << std::endl;
		/*
		Tile *lower = new Tile();
		lower->SetTileType(TileType::PARCHMENT);
		lower->SetRelativeLocation(.01,.01,.98,.28);
		lower->SetColor(RGB(255,255,255));
		display.AddObject(lower);
		*/
	}
}