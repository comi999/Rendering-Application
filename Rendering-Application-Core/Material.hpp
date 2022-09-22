#pragma once
#include "Shader.hpp"
#include "Texture.hpp"
#include "Resource.hpp"

class Material : public Resource
{
public:

	Material();
	void Apply() const;
	Shader* GetShader() const { return m_Shader; }
	void SetShader( Shader* a_Shader ) { m_Shader = a_Shader; }
	void SetTexture( Texture* a_Texture );
	Texture* GetTexture( TextureType a_Type ) const { return m_Textures[ a_Type ]; }
	void SetColour( const glm::vec4& a_Colour ) { m_Colour = a_Colour; }
	const glm::vec4& GetColour() const { return m_Colour; }


private:

	Shader*   m_Shader;
	Texture*  m_Textures[ TextureType_None ];
	glm::vec4 m_Colour;
};