#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.hpp>

Texture::Texture()
	: m_Size( 0, 0 )
	, m_Data( nullptr )
	, m_Slot( 0 )
	, m_Handle( 0 )
{ }

Texture::Texture( const std::string& a_Path )
	: m_Size( 0, 0 )
	, m_Data( nullptr )
	, m_Slot( 0 )
	, m_Handle( 0 )
{
	int Width, Height, Channels;
	m_Data = stbi_load( a_Path.c_str(), &Width, &Height, &Channels, 4 );
	m_Size.x = Width;
	m_Size.y = Height;

	glGenTextures( 1, &m_Handle );
	glBindTexture( GL_TEXTURE_2D, m_Handle );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_Size.x, m_Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

Texture::~Texture()
{
	if ( m_Handle )
	{
		glDeleteTextures( 1, &m_Handle );
	}

	if ( m_Data ) stbi_image_free( m_Data );
}

bool Texture::Bind( uint32_t a_Slot )
{
	if ( !m_Data )
	{
		return false;
	}

	glActiveTexture( GL_TEXTURE0 + a_Slot );
	glBindTexture( GL_TEXTURE_2D, m_Handle );
	m_Slot = a_Slot;
}

bool Texture::Unbind()
{
	return false;
}