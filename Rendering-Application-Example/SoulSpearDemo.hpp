#pragma once
#include <glm/glm.hpp>

#include "Component.hpp"
#include "Application.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Transform.hpp"
#include "Object.hpp"
#include "ObjectRotator.hpp"

class SoulSpearDemo : public Component
{
public:

	void OnCreate()
	{
		GetApplication()->AddComponent< ObjectRotator >( GetObject() )->Rotate = true;

		SomeTextureDiffuse = new Texture( "Resources/SoulSpear/soulspear_diffuse.tga", TextureType_Diffuse );
		SomeTextureNormal = new Texture( "Resources/SoulSpear/soulspear_normal.tga", TextureType_Normal );
		SomeTextureSpecular = new Texture( "Resources/SoulSpear/soulspear_specular.tga", TextureType_Specular );
		
		SomeShader = new Shader( "Resources/SoulSpear.shader" );
		SomeShader->Compile();

		SomeMesh = new Mesh( "Resources/SoulSpear/soulspear.obj" );

		SomeMaterial = new Material();
		SomeMaterial->SetShader( SomeShader );
		SomeMaterial->SetTexture( SomeTextureDiffuse );
		SomeMaterial->SetTexture( SomeTextureNormal );
		SomeMaterial->SetTexture( SomeTextureSpecular );
		
		Renderer* SomeRenderer = GetApplication()->AddComponent< Renderer >( GetObject() );
		SomeRenderer->SetMaterial( SomeMaterial );
		SomeRenderer->SetMesh( SomeMesh );
	}

	void OnDestroy()
	{
		delete SomeMesh;
		delete SomeShader;
		delete SomeMaterial;
		delete SomeTextureDiffuse;
		delete SomeTextureNormal;
		delete SomeTextureSpecular;
	}

private:

	Mesh*      SomeMesh;
	Shader*    SomeShader;
	Material*  SomeMaterial;
	Texture*   SomeTextureDiffuse;
	Texture*   SomeTextureNormal;
	Texture*   SomeTextureSpecular;
};