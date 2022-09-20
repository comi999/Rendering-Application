#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.hpp>

Texture::Texture()
	: m_Type( TextureType_None )
	, m_Size( 0, 0 )
	, m_Data( nullptr )
	, m_Slot( 0 )
	, m_Handle( 0 )
{ }

Texture::Texture( const std::string& a_Path, TextureType a_Type )
	: m_Type( a_Type )
	, m_Size( 0, 0 )
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
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
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