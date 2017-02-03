#ifndef TEXTURE_H__
#define TEXTURE_H__

#include <string>
#include <glad.h>
#include <map>
#include "TileType.h"


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
