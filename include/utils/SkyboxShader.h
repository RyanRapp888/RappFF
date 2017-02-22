#ifndef SKYBOX_SHADER_H__
#define SKYBOX_SHADER_H__

#include <string>
#include <glad.h>
#include "transform.h"
#include <vector>

class SkyboxShader
{
public:
	SkyboxShader(const std::string& fileName);

	void Bind();
	void Detach();
	void Update(const Transform& transform, const Camera& camera);
	GLuint GetProgramId() { return m_program; }

	virtual ~SkyboxShader();
protected:
private:
	int m_num_shaders;
	int m_num_uniforms;
	void operator=(const SkyboxShader& shader) {}
	SkyboxShader(const SkyboxShader& shader) {}

	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);

	GLuint m_program;
	std::vector< GLuint > m_shaderids;
	std::vector< GLuint > m_uniformids;
};

#endif
