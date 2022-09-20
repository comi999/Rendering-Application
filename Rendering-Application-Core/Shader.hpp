#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Texture.hpp"

class Shader
{
public:

	Shader() = default;
	Shader( const std::string& a_Path );
	bool IsCompiled() const { return m_ProgramHandle; }
	bool Compile();
	bool Decompile();
	bool Use() const;
	bool SetLights( const glm::mat4* a_Lights, uint32_t a_LightCount ) const;
	bool SetColour( const glm::vec4& a_Colour ) const;
	bool SetTexture( const Texture* a_Texture ) const;
	bool SetPVM( const glm::mat4& a_Matrix ) const;
	bool SetM( const glm::mat4& a_Matrix ) const;

private:

	std::string m_Source;
	GLuint      m_ProgramHandle;
	GLint       m_UniformLocationPVM;
	GLint       m_UniformLocationM;
	GLint       m_UniformLocationColour;
	GLint       m_UniformLocationLights;
	GLint       m_UniformLocationLightCount;
	GLint       m_UniformLocationTextures[ TextureType_None ];
};