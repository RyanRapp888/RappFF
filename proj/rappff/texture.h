#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>
#include <map>

enum class TileType
{
   BRICKS=3, DESERT=4, GRASS=0, MAINCHAR=9, MTN=7, PATH=1, PARCHMENT=8, ROCKS=6, WATER=2 
};

class Texture
{
public:

	Texture(const std::string& fileName);
	void Bind();
	virtual ~Texture();

protected:

private:
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	GLuint m_texture;
};

class TextureManager
{
   public:

   static TextureManager *GetInstance();
   bool GetTexturePtr(TileType type, Texture **texture);

   private:

   std::map<std::string, Texture *> m_lookup;
   static TextureManager *m_instance;
   static bool m_instance_created;
};

#endif
