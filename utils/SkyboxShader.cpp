#include "SkyboxShader.h"
#include <iostream>
#include <fstream>
#include <sstream>

SkyboxShader::SkyboxShader(const std::string& fileName)
{
	m_program = glCreateProgram();
	m_shaderids.emplace_back(CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER));
	m_shaderids.emplace_back(CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER));

	for (unsigned int i = 0; i < m_shaderids.size(); i++)
		glAttachShader(m_program, m_shaderids[i]);

	glBindAttribLocation(m_program, 0, "position");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");

	m_uniformids.emplace_back(glGetUniformLocation(m_program, "view"));
	m_uniformids.emplace_back(glGetUniformLocation(m_program, "projection"));
}

SkyboxShader::~SkyboxShader()
{
	for (unsigned int i = 0; i < m_shaderids.size(); i++)
	{
		glDetachShader(m_program, m_shaderids[i]);
		glDeleteShader(m_shaderids[i]);
	}

	glDeleteProgram(m_program);
}

void SkyboxShader::Bind()
{
	glUseProgram(m_program);
}

void SkyboxShader::Detach()
{
	glUseProgram(0);
}

void SkyboxShader::Update(const Transform& transform, const Camera& camera)
{
	glm::mat4 view = glm::mat4(glm::mat3(transform.GetMVP(camera)));	// Remove any translation component of the view matrix
	glm::mat4 projection = glm::perspective(90.0f, static_cast<float>(800.0 / 450.0), .1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, &projection[0][0]);
}

std::string SkyboxShader::LoadShader(const std::string& fileName)
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
		std::cerr << "Unable to load skybox shader: " << fileName << std::endl;
	}

	return output;
}

void SkyboxShader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
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

GLuint SkyboxShader::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}
