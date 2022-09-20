#include "Material.hpp"

Material::Material()
	: m_Shader( nullptr )
	, m_Textures{ nullptr }
	, m_Colour( 1.0f )
{ }

void Material::Apply() const
{
	if ( !m_Shader || !m_Shader->IsCompiled() )
	{
		return;
	}

	m_Shader->Use();

	// Set textures
	for ( uint32_t i = 0; i < TextureType_None; ++i )
	{
		m_Shader->SetTexture( m_Textures[ i ] );
	}

	// Set colour
	m_Shader->SetColour( m_Colour );
}

void Material::SetTexture( Texture* a_Texture )
{
	if ( !a_Texture )
	{
		return;
	}

	m_Textures[ a_Texture->GetType() ] = a_Texture;
}