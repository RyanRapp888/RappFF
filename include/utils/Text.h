#ifndef _Text_H__
#define _Text_H__

#include <iostream>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad.h>
#include "GLFW\glfw3.h"
#include <map>

using namespace std;

enum class TextAlignType
{
	LEFT,
	CENTER,
	RIGHT
};

class GlyphData 
{
public:
	GlyphData() :bitmap_buffer(nullptr) {}
	char c;
	int size;
	int bitmap_width;
	int bitmap_rows;
	unsigned char *bitmap_buffer;
	FT_Vector advance;
	int bitmap_left;
	int bitmap_top;
};

class Text 
{
public:
	
	Text(int screenwidth, int screenheight);
	~Text();
	void Init(GLuint prog, int size);
	void Render(std::string text, float x, float y, TextAlignType align);
	void FontOpt(int opt, int value);

private:

	FT_Library m_library;
	FT_Face m_face;
	FT_GlyphSlot m_glyphslot;
	bool GetGlyph(char c,GlyphData &returned_glyph);
	
	int m_currentSize;
	GLint m_shaderProgram;
	GLuint m_vao;
	GLuint m_vbo;
	GLint m_fontCoords;
	GLuint m_textureid;
	std::map<char,GlyphData> m_glyphlookup;
	int m_screenWidth;
	int m_screenHeight;
};

#endif