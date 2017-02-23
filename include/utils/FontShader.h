#ifndef FONTSHADER_H__
#define FONTSHADER_H__

#include <string>
#include <glad.h>
#include "transform.h"
#include <vector>

class FontShader
{

public:

	FontShader(const std::string& fileName);
	void Bind();
	void Detach();
	GLuint GetProgramId() { return m_program; }
	virtual ~FontShader();

protected:

private:
	int m_num_shaders;
	int m_num_uniforms;
	void operator=(const FontShader& shader) {}
	FontShader(const FontShader& shader) {}
	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);

	GLuint m_program;
	std::vector< GLuint > m_shaderids;
	std::vector< GLuint > m_uniformids;
};

#endif
