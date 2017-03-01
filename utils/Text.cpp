#include "Text.h"

Text::Text(int screenwidth,int screenheight) 
{
	m_screenWidth = screenwidth;
	m_screenHeight = screenheight;
	m_currentSize = 10;
}

Text::~Text()
{
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_library);
}

void Text::Init(GLuint shaderid, int size)
{
	if (FT_Init_FreeType(&m_library))
	{
		printf("Couldn't init freetype\n");
		exit(1);
	}
	
	if (FT_New_Face(m_library, "res\\CarbonBlock.ttf", 0, &m_face))
	{
		printf("couldn't open font\n");
		exit(1);
	}
	
	m_currentSize = size;
	FT_Set_Pixel_Sizes(m_face, 0, size);
	m_shaderProgram = shaderid;

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	m_fontCoords = glGetAttribLocation(m_shaderProgram, "fontCoords");
	glEnableVertexAttribArray(m_fontCoords);
	glVertexAttribPointer(m_fontCoords, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void Text::Render(std::string text, float x, float y, TextAlignType align)
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureid);
	glUseProgram(m_shaderProgram);
	
	glUniform4f(glGetUniformLocation(m_shaderProgram,"color"), .5f, .6f, .7f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float screenx = 2.0f / m_screenWidth;
	float screeny = 2.0f / m_screenHeight;
	float totalWidth = 0.0f;
	int index = 0;
	
	for (const auto curchar : text)
	{ 
		GlyphData curglyph;
		GetGlyph(curchar,curglyph);
		totalWidth += (curglyph.advance.x >> 6) * screenx; // add the horizontal advance value to the totalWidth
	}

	index = 0;
	for (const auto curchar2 : text)
	{ 
		GlyphData curglyph;
		GetGlyph(curchar2,curglyph);
		
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &m_textureid);
		glBindTexture(GL_TEXTURE_2D, m_textureid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, curglyph.bitmap_width,
			         curglyph.bitmap_rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE,
					 curglyph.bitmap_buffer);

		float x2 = x + curglyph.bitmap_left * screenx;
		float y2 = -y - curglyph.bitmap_top * screeny;
		float w = curglyph.bitmap_width * screenx;
		float h = curglyph.bitmap_rows * screeny;

		if (align == TextAlignType::CENTER)
		{
			x2 -= totalWidth / 2;
		}
		else if (align == TextAlignType::RIGHT)
		{
			x2 -= totalWidth;
		}

		GLfloat box[4][4] = {
			{ x2, -y2, 0, 0 },
			{ x2 + w, -y2, 1, 0 },
			{ x2, -y2 - h, 0, 1 },
			{ x2 + w, -y2 - h, 1, 1 }
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (curglyph.advance.x >> 6) * screenx;
		y += (curglyph.advance.y >> 6) * screeny;

		glDeleteTextures(1, &m_textureid);
	}
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_BLEND);
}

bool Text::GetGlyph(char c, GlyphData &returned_glyph) 
{
	GlyphData result;
	auto iter = m_glyphlookup.find(c);
	if(iter != m_glyphlookup.end()) 
	{
		returned_glyph = iter->second;
		return true;
	}
	
	if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
	{
		printf("Freetype is unable to load char: %c\n", c);
		return false;
	}

	m_glyphslot = m_face->glyph;
	returned_glyph.bitmap_buffer = new unsigned char[m_glyphslot->bitmap.rows * m_glyphslot->bitmap.width * 4];
	memcpy(returned_glyph.bitmap_buffer, m_glyphslot->bitmap.buffer,
		   m_glyphslot->bitmap.rows * m_glyphslot->bitmap.width * 4);

	returned_glyph.bitmap_width = m_glyphslot->bitmap.width;
	returned_glyph.bitmap_rows = m_glyphslot->bitmap.rows;
	returned_glyph.bitmap_left = m_glyphslot->bitmap_left;
	returned_glyph.bitmap_top = m_glyphslot->bitmap_top;
	returned_glyph.advance = m_glyphslot->advance;
	returned_glyph.c = c;
	returned_glyph.size = m_currentSize;
	m_glyphlookup[c] = returned_glyph;
	return true;
}

void Text::FontOpt(int opt, int value) 
{
	FT_Set_Pixel_Sizes(m_face, 0, value);
	m_currentSize = value;
}