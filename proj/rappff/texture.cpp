#include "texture.h"
#include "stb_image.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;
bool TextureManager::m_instance_created = false;
TextureManager *TextureManager::m_instance = 0;

std::map<TileType, std::string> tiletype_lookup 
{ 
   {TileType::BRICKS, "../../res/bricks.jpg"},
   {TileType::DESERT, "../../res/desert.jpg"},
   {TileType::GRASS, "../../res/grass.png"},
   {TileType::MAINCHAR, "../../res/cat.png"},
   {TileType::MTN, "../../res/mtn.png"},
   {TileType::PATH, "../../res/grass1.png"},
   {TileType::PARCHMENT, "../../res/parchment.jpg"}, 
   {TileType::ROCKS, "../../res/rocks.jpg"},
   {TileType::WATER, "../../res/Water.png"},
   {TileType::FAIRY, "../../res/fairy.jpg"},
   {TileType::MERMAID, "../../res/mermaid.jpg"},
   {TileType::OCTOPUS, "../../res/octopus.png"},
   {TileType::JELLYBEAN, "../../res/jellybean.jpg"},
   {TileType::PRINCESS, "../../res/princess.jpg"},
   {TileType::BLACK, "../../res/black.jpg"}
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
      auto iter2 = m_lookup.find( iter->second );
      if(iter2 != m_lookup.end())
      {
         *texture = iter2->second;
         return true;
      }
   }

   *texture = new Texture(tiletype_lookup[type]);
   m_lookup[tiletype_lookup[type]] = *texture;
   return false;
}

Texture::Texture(const std::string& fileName)
{
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
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
