#pragma once
#include "Shader.hpp"
#include "Texture.hpp"

class Material
{
public:

	Material();
	Shader* GetShader() const { return m_Shader; }
	void SetShader( Shader* a_Shader ) { m_Shader = a_Shader; }
	void SetDiffuse( Texture* a_Texture ) { m_Diffuse = a_Texture; }
	Texture* GetDiffuse() const { return m_Diffuse; }

private:

	Shader* m_Shader;
	Texture* m_Diffuse;
};