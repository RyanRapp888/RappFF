
#include <glad.h>
#include "TiledGameBoard.h"
#include "Shader.h"
#include "GameMap.h"
#include "Character.h"
#include "Display.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

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
	int m_xrot, m_yrot, m_zrot;
};

Sandbox abc;

void Sandbox::KeyHandler(int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case(GLFW_KEY_X):
		m_xrot++;
		break;
	case(GLFW_KEY_Y):
		m_yrot++;
		break;
	case(GLFW_KEY_Z):
		m_zrot++;
	
	default:
		break;
	}
}

void Sandbox::Play()
{
	m_display_ptr = new Display(800, 600, "Sandbox");
	if (m_display_ptr == nullptr) return;
	
	/*
	Mesh *theMesh;
	MeshManager *mm = MeshManager::GetInstance();
	mm->GetMeshPtr(std::string("res\\mtn.robj"),&theMesh);
	theMesh->UseTexture(std::string("res\\mtn.png"));
	m_display_ptr->AddObject(theMesh);
	*/
	Tile tmptile;
	tmptile.SetColor(RGB(255, 255, 255));
	tmptile.SetTileType(TileType::MTN);
	m_display_ptr->AddObject(&tmptile);

	Shader testShader("res\\basicShader");
	testShader.Bind();
	Transform transform;
	Camera camera(glm::vec3(0.0f, 0.0f, -2.0f), 70.0f, 800/600, 0.1f, 100.0f);
	
	
	while (!m_display_ptr->WindowShouldClose())
	{
		m_display_ptr->Clear(.9, .9, .9, 1);
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

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	abc.KeyHandler(key, scancode, action, mods);
}

void Sandbox::Interact(int x, int y)
{
	int aa = 0;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main()
{
	SetKeyCallbackFunc(KeyCallback);
	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	
	abc.Play();
}