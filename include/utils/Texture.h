#ifndef TEXTURE_H__
#define TEXTURE_H__

#include <glad.h>
#include "TileType.h"

class Texture
{
	friend class TextureManager;
public:
	// To get a new texture, use the texture manager class below
	void Bind();
	void UnBind();
	virtual ~Texture();

protected:

private:
	Texture(const std::string& fileName);
	Texture(const Texture& texture) {}
	void operator=(const Texture& texture) {}

	GLuint m_texture;
};

class TextureManager
{
   public:

   static TextureManager *GetInstance();
   bool GetTexturePtr(const std::string &filename, Texture **texture);

   private:

   std::map<std::string, Texture *> m_lookup;
   static TextureManager *m_instance;
   static bool m_instance_created;
};

#endif
