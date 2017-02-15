#include "Texture.h"
#include "StbImage.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

bool TextureManager::m_instance_created = false;

TextureManager *TextureManager::m_instance = 0;

static std::map<TileType, std::string> tiletype_lookup
{
	{ TileType::BLACK, "res\\black.jpg" },
	{ TileType::BRICKS, "res\\bricks.jpg" },
	{ TileType::DESERT, "res\\desert.jpg" },
	{ TileType::EMMY, "res\\emmy.png"},
	{ TileType::FAIRY, "res\\fairy.png"},
	{ TileType::GRASS, "res\\grass.png" },
	{ TileType::GRASS2, "res\\grass2.png" },
	{ TileType::JELLYBEAN, "res\\jellybean.jpg" },
	{ TileType::MAINCHAR, "res\\cat.png" },
	{ TileType::MERMAID, "res\\Mermaid.png" },
	{ TileType::MTN, "res\\mtn.png" },
	{ TileType::MTNSNOW, "res\\mtnsnow.png" },
	{ TileType::MUD, "res\\mud.png" },
	{ TileType::OCTOPUS, "res\\octopus.png" },
	{ TileType::PARCHMENT, "res\\parchment.jpg" },
	{ TileType::PLANK, "res\\Plank.png" },
	{ TileType::PRINCESS, "res\\princess.png" },
	{ TileType::ROCKS, "res\\rocks.jpg" },
	{ TileType::ROOF, "res\\roof.png" },
	{ TileType::WATER, "res\\Water.png" },
	{ TileType::WOOD, "res\\Wood.png"}
};


TextureManager *TextureManager::GetInstance()
{
   if(m_instance_created)
   {
      return m_instance; 
   }
   
   m_instance = new TextureManager();
   m_instance_created = true;
   return m_instance;  
}

bool TextureManager::GetTexturePtr(TileType type, Texture **texture)
{
   auto iter = tiletype_lookup.find(type);
   if(iter != tiletype_lookup.end())
   { 
	   if(GetTexturePtr(iter->second, texture))
	   {
		  return true;
      }
   }
   
   return false;
}

bool TextureManager::GetTexturePtr(const std::string &filename, Texture **texture)
{
	auto iter2 = m_lookup.find(filename);
	if (iter2 != m_lookup.end())
	{
		*texture = iter2->second;
		return true;
	}

	*texture = new Texture(filename);
	if (*texture == nullptr) return false;
	m_lookup[filename] = *texture;
	return true;
}

Texture::Texture(const std::string& fileName)
{
	// don't use this directly, go through the TextureManager
   int width, height, numComponents;
   unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

   if(data == NULL)
      std::cerr << "Unable to load texture: " << fileName << std::endl;
        
   glGenTextures(1, &m_texture);
   glBindTexture(GL_TEXTURE_2D, m_texture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   GLfloat max_aniso = 0.0f;
   glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
   glGenerateMipmap(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, 0);
   stbi_image_free(data);
}

Texture::~Texture()
{
   glDeleteTextures(1, &m_texture);
}

void Texture::Bind()
{
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::UnBind()
{
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
