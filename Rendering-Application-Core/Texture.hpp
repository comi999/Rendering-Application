#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Texture
{
public:

	Texture();
	Texture( const std::string& a_Path );
	~Texture();
	bool Bind( uint32_t a_Slot );
	bool Unbind();
	GLuint GetHandle() const { return m_Handle; }
	glm::vec2 GetSize() const { return m_Size; }
	const unsigned char* GetData() const { return m_Data; }

private:

	glm::uvec2     m_Size;
	unsigned char* m_Data;
	GLuint         m_Handle;
	uint32_t       m_Slot;
};