#include <glad.h>
#include "TheGame.h"
#include "TiledGameBoard.h"
#include "FightMode.h"
#include "Shader.h"
#include "SkyboxShader.h"
#include "FontShader.h"
#include "Camera.h"
#include "StbImage.h"
#include "Text.h"
#include <sstream>
#include "RandUtils.h"
#include "Toc.h"

//bool SetUpSkybox(GLuint &cube_vao, GLuint &cube_text_id);

TheGame::TheGame() :
	m_display_ptr(nullptr),
	m_cur_mode(GameMode::NewOrLoadMode),
	m_mapwalkingmode_ws(nullptr),
	m_fightmode_ws(nullptr)
{
}

void TheGame::Play()
{
	int initial_ww(1600);
	int initial_wh(900);
	m_display_ptr = new Display(initial_ww, initial_wh, "Wheat Ridge");
	Transform maintransform;
	double aspect(initial_ww / static_cast<double>(initial_wh));
	Camera maincamera(glm::vec3(0, 0, .8), 80.0f, aspect, 0.1f, 30000);
	if (m_display_ptr == nullptr) return;

	FontShader fontShader("res\\fontShader");
	Text main_texthandler(m_display_ptr->GetWinWidth(), m_display_ptr->GetWinHeight());
	main_texthandler.Init(fontShader.GetProgramId(), 35);

	m_mapwalkingmode_ws = new TiledGameBoard(m_display_ptr, .01, .01, .98, .98);
	Shader basicShader("res\\basicShader");

	//SkyboxShader skyboxShader("res\\skyboxshader");
	basicShader.Bind();
	m_mapwalkingmode_ws->SetTileDetails(24, 24);
	m_display_ptr->AddWindowSection(m_mapwalkingmode_ws);

	m_neworload_ws = new NewOrLoadMode(m_display_ptr, 0, 0, 1, 1);
	m_neworload_ws->SetPrimaryShader(basicShader.GetProgramId());
	m_neworload_ws->SetTextHandler(&main_texthandler);
	m_display_ptr->AddWindowSection(m_neworload_ws);

	m_partybuild_ws = new PartyBuildMode(m_display_ptr, 0, 0, 1, 1);
	m_partybuild_ws->SetPrimaryShader(basicShader.GetProgramId());
	m_partybuild_ws->SetTextHandler(&main_texthandler);
	m_display_ptr->AddWindowSection(m_partybuild_ws);

	m_fightmode_ws = new FightMode(m_display_ptr, 0, 0, 1, 1);
	m_fightmode_ws->SetPrimaryShader(basicShader.GetProgramId());
	m_fightmode_ws->SetTextHandler(&main_texthandler);
	m_display_ptr->AddWindowSection(m_fightmode_ws);

	m_xrot = -15;
	m_yrot = 0;
	m_zrot = 0;
	RGB background(255, 255, 255);

	/*
	GLuint skybox_VAO;
	GLuint skybox_texture;
	if (!SetUpSkybox(skybox_VAO, skybox_texture))
	{
		std::cout << "Skybox failure" << std::endl;
		return;
	}
	*/
	Tile thotbubs;
	thotbubs.SetTileType(TileType::BLACK);
	thotbubs.SetRelativeLocation(0, 0, 1, .2);
	m_display_ptr->AddObject(&thotbubs);

	SetCurMode(GameMode::NewOrLoadMode);

	while (!m_display_ptr->WindowShouldClose())
	{
		m_display_ptr->Clear(
			background.GetRed() / 255,
			background.GetGreen() / 255.0,
			background.GetBlue() / 255.0,
			1);

		/*
		ostringstream location;
		location << "(" << m_chars[0]->GetX() << "," << m_chars[0]->GetY() << ")";
		main_texthandler.Render(location.str(), -.9f, .9f, TextAlignType::CENTER);
		*/

		basicShader.Bind();
		maintransform.SetRot(glm::vec3(m_xrot, m_yrot, m_zrot));

		if (m_cur_mode == GameMode::FightMode)
		{
			maintransform.SetRot(glm::vec3(0, 0, 0));
			//m_mapwalkingmode_ws->Disable();
			//m_fightmode_ws->Enable();
			basicShader.Ortho(maintransform);
		}
		else if (m_cur_mode == GameMode::MapWalkingMode)
		{
			basicShader.Bind();
			//m_fightmode_ws->Disable();
			//m_mapwalkingmode_ws->Enable();
			basicShader.Update(maintransform, maincamera);
		}
		else if (m_cur_mode == GameMode::NewOrLoadMode)
		{
			maintransform.SetRot(glm::vec3(0, 0, 0));
			basicShader.Ortho(maintransform);
		}
		else if (m_cur_mode == GameMode::PartyBuildMode)
		{
			maintransform.SetRot(glm::vec3(0, 0, 0));
			basicShader.Ortho(maintransform);
		}

		m_display_ptr->Refresh();
		if (m_chatting)
		{

			if (m_chat_char != nullptr)
			{
				std::vector<std::string> dialogue = m_chat_char->GetDialogueLines();

				float textorigx(0), textorigy(0);
				if (m_chat_prox == ProxRel::FOUND_TO_LEFT)
				{
					textorigx = -.4;
					textorigy = 0;
				}
				else if (m_chat_prox == ProxRel::FOUND_TO_RIGHT)
				{
					textorigx = .15;
					textorigy = .0;
				}
				else if (m_chat_prox == ProxRel::FOUND_ABOVE)
				{
					textorigx = 0;
					textorigy = .2 + .12 * dialogue.size();
				}
				else if (m_chat_prox == ProxRel::FOUND_BELOW)
				{
					textorigx = 0;
					textorigy = -.3;
				}

				for (int aa = 0; aa < dialogue.size(); aa++)
				{
					main_texthandler.Render(dialogue[aa], textorigx, textorigy, TextAlignType::LEFT);
					textorigy -= .12;
				}
			}
		}
		m_display_ptr->SwapBuffers();
		glfwPollEvents();
	}
}

void TheGame::Interact(int x, int y)
{
	Character *foundchar(nullptr);
	ProxRel proximity;

	GameMap *map_ptr = GameMap::GetInstance();
	bool found = map_ptr->FindTouchingCharacter(
		m_chars[0]->GetX(),
		m_chars[0]->GetY(),
		foundchar, proximity);

	if (found)
	{
		m_chatting = !m_chatting;
		if (m_chatting)
		{
			m_chat_char = foundchar;
			m_chat_prox = proximity;
		}
		else
		{
			m_chat_char = nullptr;
		}
	}
}

void TheGame::SetCurMode(GameMode dat)
{
	if (dat == GameMode::FightMode)
	{
		m_mapwalkingmode_ws->Disable();
		m_neworload_ws->Disable();
		m_partybuild_ws->Disable();
		m_fightmode_ws->StartFight();
		m_fightmode_ws->Enable();
	}
	else if (dat == GameMode::MapWalkingMode)
	{
		m_fightmode_ws->Disable();
		m_neworload_ws->Disable();
		m_partybuild_ws->Disable();
		m_mapwalkingmode_ws->Enable();
	}
	else if (dat == GameMode::NewOrLoadMode)
	{
		m_mapwalkingmode_ws->Disable();
		m_partybuild_ws->Disable();
		m_fightmode_ws->Disable();
		m_neworload_ws->Enable();
	}
	else if (dat == GameMode::PartyBuildMode)
	{
		m_mapwalkingmode_ws->Disable();
		m_fightmode_ws->Disable();
		m_neworload_ws->Disable();
		m_partybuild_ws->Enable();
	}
	m_cur_mode = dat;
}

void TheGame::KeyHandler(int key, int scancode, int action, int mods)
{
	if (m_cur_mode == GameMode::FightMode)
	{
		if (m_fightmode_ws != nullptr)
		{
			m_fightmode_ws->HandleKey(key, scancode, action, mods);
			if (m_fightmode_ws->FightEnded())
			{
				SetCurMode(GameMode::MapWalkingMode);
			}
		}
	}
	else if (m_cur_mode == GameMode::NewOrLoadMode)
	{
		if (m_neworload_ws != nullptr)
		{
			m_neworload_ws->HandleKey(key, scancode, action, mods);
			if (m_neworload_ws->IsScreenComplete())
			{
				std::string save_file_name = m_neworload_ws->GetSaveFileName();
				if (save_file_name.size() == 0)
				{
					SetCurMode(GameMode::PartyBuildMode);
				}
				else
				{
					LoadGameFromFile(save_file_name);
					SetCurMode(GameMode::MapWalkingMode);
				}
			}
		}
	}
	else if (m_cur_mode == GameMode::PartyBuildMode)
	{
		if (m_partybuild_ws != nullptr)
		{
			m_partybuild_ws->HandleKey(key, scancode, action, mods);
			if (m_partybuild_ws->IsPartyBuildComplete())
			{
				LoadGameFromPartyBuild();
				std::string saveloc = m_partybuild_ws->GetSaveLoc();
				SaveGameToFile(saveloc);
				SetCurMode(GameMode::MapWalkingMode);
			}
		}
	}
	else if (m_cur_mode == GameMode::MapWalkingMode)
	{
		if (action == GLFW_PRESS)
		{
			int xx = m_chars[0]->GetX();
			int yy = m_chars[0]->GetY();
			int dir(1);
			if (mods == GLFW_MOD_SHIFT) dir *= -1;
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
				xx--;
				took_step = true;
				break;
			case(GLFW_KEY_S):
			case(GLFW_KEY_DOWN):
				yy--;
				took_step = true;
				break;
			case(GLFW_KEY_D):
			case(GLFW_KEY_RIGHT):
				xx++;
				took_step = true;
				break;
			case(GLFW_KEY_X):
				m_xrot -= dir * 5;
				std::cout << "rot(" << m_xrot << "," << m_yrot << "," << m_zrot << ")\n";
				break;
			case(GLFW_KEY_Y):
				m_yrot -= dir * 5;
				std::cout << "rot(" << m_xrot << "," << m_yrot << "," << m_zrot << ")\n";
				break;
			case(GLFW_KEY_Z):
				m_zrot -= dir * 5;
				std::cout << "rot(" << m_xrot << "," << m_yrot << "," << m_zrot << ")\n";
			case(GLFW_KEY_ENTER):
				Interact(m_chars[0]->GetX(), m_chars[0]->GetY());
			default:
				break;
			}

			if (took_step)
			{
				if (!m_chatting && m_chars[0]->SetLocation(xx, yy))
				{
					GameMap *gamemap_ptr = GameMap::GetInstance();
					int randy = get_rand_0_99();
					int mob_odds = gamemap_ptr->GetMobOdds(xx, yy);
					if (randy <= mob_odds)
					{
						std::cout << "FIGHTY MODE!!!" << std::endl;
						//SetCurMode(GameMode::FightMode);

					}
					std::cout << randy << " out of " << mob_odds << std::endl;
					std::cout << "(" << xx << "," << yy << ")" << std::endl;
				}
			}
		}

	}
}

bool TheGame::LoadGameFromPartyBuild()
{
	m_chars.resize(4);
	for (unsigned int aa = 0; aa < m_chars.size(); aa++)
	{
		m_chars[aa] = new Character();
	}

	GameMap *gamemap_ptr = GameMap::GetInstance();
	if (!gamemap_ptr->LoadGameMap(256, 256))
	{
		std::cout << "Error: Could not load game map" << std::endl;
	}
	
	std::vector<std::string> char_names;
	std::vector<TileType> char_tile_types;
	m_partybuild_ws->GetPartyCharacterNames(char_names);
	m_partybuild_ws->GetPartyCharacterTypes(char_tile_types);

	for (int aa = 0; aa < char_names.size() && aa < 4; aa++)
	{
		int locx(20), locy(5);
		m_chars[aa]->SetName(char_names[aa]);
		m_chars[aa]->SetCharacterType(TileTypeToCharType(char_tile_types[aa]));
		m_chars[aa]->SetLocation(locx, locy);
	}

		gamemap_ptr->AttachMainCharacter(m_chars[0]);
		gamemap_ptr->AddToHeroParty(m_chars[0]);
		gamemap_ptr->AddToHeroParty(m_chars[1]);
		gamemap_ptr->AddToHeroParty(m_chars[2]);
		gamemap_ptr->AddToHeroParty(m_chars[3]);
		
	for (int bb = 0; bb < m_chars.size(); bb++)
	{
		gamemap_ptr->AttachCharacter(m_chars[bb]);
	}

	return true;
}

bool TheGame::LoadGameFromFile(const std::string &filename)
{
	m_chars.resize(9);
	for (unsigned int aa = 0; aa < m_chars.size(); aa++)
	{
		m_chars[aa] = new Character();
	}

	GameMap *gamemap_ptr = GameMap::GetInstance();
	if (!gamemap_ptr->LoadGameMap(256, 256))
	{
		std::cout << "Error: Could not load game map" << std::endl;
	}

	Toc save_game_helper(filename);
	if (!save_game_helper.HasError())
	{
		save_game_helper.LoadObjects();
	}
	else
	{
		std::cout << "Error: Failed to load objects from " << filename << std::endl;
	}

	std::vector<ObjData> saved_characters;
	save_game_helper.GetObjData("character", saved_characters);

	int idx(0);
	for (auto &cur : saved_characters)
	{
		std::string charname = cur.m_str_data["char_name"];
		std::string chartype = cur.m_str_data["char_type"];
		int locx(-1), locy(-1);
		locx = cur.m_int_data["charlocx"];
		locy = cur.m_int_data["charlocy"];
		if (!is_blank(charname) && !is_blank(chartype) &&
			locx >= 0 && locy >= 0)
		{
			m_chars[idx]->SetName(charname);
			m_chars[idx]->SetCharacterType(chartype);
			m_chars[idx]->SetLocation(locx, locy);
			idx++;
		}
	}

	if (idx >= 3)
	{
		gamemap_ptr->AttachMainCharacter(m_chars[0]);
		gamemap_ptr->AddToHeroParty(m_chars[0]);
		gamemap_ptr->AddToHeroParty(m_chars[1]);
		gamemap_ptr->AddToHeroParty(m_chars[2]);
		gamemap_ptr->AddToHeroParty(m_chars[3]);
	}
	else
	{
		std::cout << "Error: There must be at least 4 characters in  " << filename << std::endl;
	}

	for (int bb = 0; bb < m_chars.size(); bb++)
	{
		gamemap_ptr->AttachCharacter(m_chars[bb]);
	}

	return true;
}

static void GetGameObjTypes(std::vector<ObjType> &objtypes)
{
	ObjType game_character("character");
	game_character.AddAttribute("char_name", "str");
	game_character.AddAttribute("char_type", "str");
	game_character.AddAttribute("charlocx", "int");
	game_character.AddAttribute("charlocy", "int");
	objtypes.emplace_back(game_character);

	ObjType game_weapon("weapon");
	game_weapon.AddAttribute("weap_name", "str");
	game_weapon.AddAttribute("weap_attack", "int");
	objtypes.emplace_back(game_weapon);
}

bool TheGame::SaveGameToFile(const std::string &filename) const
{
	std::vector<ObjType> m_types;
	GetGameObjTypes(m_types);

	std::ofstream outfile(filename);
	outfile << "starttypes" << std::endl;
	for (auto cur_type : m_types)
	{
		outfile << cur_type.AsString();
	}
	outfile << "endtypes" << std::endl;
	outfile << std::endl;

	outfile << "startdata" << std::endl;
	outfile << "character Moochie OCTOPUS 132 5" << std::endl;
	outfile << "character Glarbo MERMAID 132 5" << std::endl;
	outfile << "character TheSpleen JELLYBEAN 132 5" << std::endl;
	outfile << "character DocRockerton OCTOPUS 132 5" << std::endl;
	outfile << "enddata" << std::endl;
	outfile << std::endl;

	return true;

}


/*
bool SetUpSkybox(GLuint &cube_vao, GLuint &cube_text_id)
{
	glGenTextures(1, &cube_text_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_text_id);
	int width, height, numComponents;
	unsigned char* image;
	std::vector<std::string> box_img_names =
	{
		"res\\right.jpg",
		"res\\left.jpg",
		"res\\top.jpg",
		"res\\bottom.jpg",
		"res\\back.jpg",
		"res\\front.jpg"
	};

	for (GLuint ii = 0; ii < box_img_names.size(); ii++)
	{
		int width, height, numComponents;
		image = stbi_load((box_img_names[ii]).c_str(), &width, &height, &numComponents, 3);

		if (image == NULL)
		{
			std::cerr << "Unable to load texture: " << box_img_names[ii] << std::endl;
			return false;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + ii, 0, GL_RGB, width,
			height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	IndexedModel model;
	model = OBJModel("res\\skybox.robj").ToIndexedModel();

	GLuint skyboxVBO;
	glGenVertexArrays(1, &cube_vao);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(cube_vao);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.m_positions[0]) * model.m_positions.size(),
		&(model.m_positions[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	return true;
}

//glDepthMask(GL_FALSE);
//skyboxShader.Bind();
//skyboxShader.Update(maintransform, maincamera);
//glBindVertexArray(skybox_VAO);
//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
//glDrawArrays(GL_TRIANGLES, 0, 36);
//glBindVertexArray(0);
//glDepthMask(GL_TRUE);
//skyboxShader.Detach();


*/
