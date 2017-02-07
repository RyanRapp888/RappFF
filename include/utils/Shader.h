#ifndef SHADER_H__
#define SHADER_H__

#include <string>
#include <glad.h>
#include "transform.h"
#include <vector>
#include "Shader.h"

class Shader
{
public:
	Shader(const std::string& fileName);

	void Bind();
	void Detach();
	void Update(const Transform& transform, const Camera& camera);
	GLuint GetProgramId() { return m_program; }

	virtual ~Shader();
protected:
private:
	int m_num_shaders;
	int m_num_uniforms;
	void operator=(const Shader& shader) {}
	Shader(const Shader& shader) {}

	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);

	GLuint m_program;
	std::vector< GLuint > m_shaderids;
	std::vector< GLuint > m_uniformids;
};

#endif
