#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum TextureType : uint32_t
{
	TextureType_Diffuse,
	TextureType_Normal,
	TextureType_Specular,
	TextureType_None
};

class Texture
{
public:

	Texture();
	Texture( const std::string& a_Path, TextureType a_Type );
	~Texture();
	uint32_t GetSlot() const { return m_Slot; }
	GLuint GetHandle() const { return m_Handle; }
	TextureType GetType() const { return m_Type; }
	glm::vec2 GetSize() const { return m_Size; }
	const unsigned char* GetData() const { return m_Data; }

private:

	TextureType    m_Type;
	glm::uvec2     m_Size;
	unsigned char* m_Data;
	GLuint         m_Handle;
	uint32_t       m_Slot;
};