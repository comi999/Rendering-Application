#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Shader
{
public:

	Shader() = default;
	Shader( const std::string& a_Path );
	bool IsCompiled() const { return m_ProgramHandle; }
	bool Compile();
	bool Decompile();
	bool Use() const;
	bool SetUniformDiffuse( int32_t a_Handle ) const;
	bool SetUniformPVM( const glm::mat4& a_Matrix ) const;
	bool SetUniformM( const glm::mat4& a_Matrix ) const;

private:

	std::string m_Source;
	GLuint      m_ProgramHandle;
	GLint       m_UniformLocationDiffuse;
	GLint       m_UniformLocationPVM;
	GLint       m_UniformLocationM;
};