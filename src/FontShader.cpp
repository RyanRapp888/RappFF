#include "FontShader.h"
#include <iostream>
#include <fstream>
#include <sstream>

FontShader::FontShader(const std::string& fileName)
{
	m_program = glCreateProgram();
	m_shaderids.emplace_back(CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER));
	m_shaderids.emplace_back(CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER));

	for (size_t i = 0; i < m_shaderids.size(); i++)
		glAttachShader(m_program, m_shaderids[i]);

	glBindAttribLocation(m_program, 0, "fontCoords");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");

	m_uniformids.emplace_back(glGetUniformLocation(m_program, "color"));
	
}

FontShader::~FontShader()
{
	for (size_t i = 0; i < m_shaderids.size(); i++)
	{
		glDetachShader(m_program, m_shaderids[i]);
		glDeleteShader(m_shaderids[i]);
	}

	glDeleteProgram(m_program);
}

void FontShader::Bind()
{
	glUseProgram(m_program);
}

void FontShader::Detach()
{
	glUseProgram(0);
}

GLuint FontShader::GetProgramId() { return m_program; }

std::string FontShader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load FontShader: " << fileName << std::endl;
	}

	return output;
}

void FontShader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

GLuint FontShader::CreateShader(const std::string& text, GLenum type)
{
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = static_cast<GLint>(text.length());

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}
