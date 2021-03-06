#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& fileName)
{
	m_program = glCreateProgram();
	m_shaderids.emplace_back(CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER));
	m_shaderids.emplace_back(CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER));

	for(size_t i = 0; i < m_shaderids.size(); i++)
		glAttachShader(m_program, m_shaderids[i]);

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");
	glBindAttribLocation(m_program, 3, "trans");

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Invalid shader program");

	m_uniformids.emplace_back(glGetUniformLocation(m_program, "MVP"));
	m_uniformids.emplace_back(glGetUniformLocation(m_program, "Normal"));
	m_uniformids.emplace_back(glGetUniformLocation(m_program, "lightDirection"));
}

Shader::~Shader()
{
	for(size_t i = 0; i < m_shaderids.size(); i++)
    {
        glDetachShader(m_program, m_shaderids[i]);
        glDeleteShader(m_shaderids[i]);
    }

	glDeleteProgram(m_program);
}

void Shader::Bind()
{
	glUseProgram(m_program);
}

void Shader::Detach()
{
	glUseProgram(0);
}

void Shader::Ortho(const Transform& transform)
{
	glm::mat4 mvp(1);
	glm::mat4 normal(transform.GetModel());
	
	glUniformMatrix4fv(m_uniformids[0], 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(m_uniformids[1], 1, GL_FALSE, &normal[0][0]);
	//light-direction
	glUniform3f(m_uniformids[2], 0.0f, 0.0f, -1.0f);
}

void Shader::Update(const Transform& transform, const Camera& camera)
{
	glm::mat4 VP = camera.GetViewProjection();
	glm::mat4 MVP = transform.GetMVP(VP);
	glm::mat4 Normal = transform.GetModel();

	static int anus(0);

	if (anus % 500 == 0)
	{
		printf("****************\nMVP\n\n");
		for (int aa = 0; aa < 4; aa++)
		{
			printf("%8.5f,%8.5f,%8.5f,%8.5f\n", MVP[aa][0], MVP[aa][1], MVP[aa][2], MVP[aa][3]);
		}

		printf("****************\nNormal\n\n");
		for (int aa = 0; aa < 4; aa++)
		{
			printf("%8.5f,%8.5f,%8.5f,%8.5f\n", Normal[aa][0], Normal[aa][1], Normal[aa][2], Normal[aa][3]);
		}

	}
	anus++;

	glUniformMatrix4fv(m_uniformids[0], 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(m_uniformids[1], 1, GL_FALSE, &Normal[0][0]);
	//light-direction
	glUniform3f(m_uniformids[2], 0.0f, 0.0f, -1.0f);
}

std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}

GLuint Shader::CreateShader(const std::string& text, GLenum type)
{
    GLuint shader = glCreateShader(type);

    if(shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

    const GLchar* p[1];
    p[0] = text.c_str();
    GLint lengths[1];
    lengths[0] = static_cast<GLint> (text.length());

    glShaderSource(shader, 1, p, lengths);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

    return shader;
}
