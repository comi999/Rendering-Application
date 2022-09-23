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
#include "Skeleton.hpp"

class ObjectRotator : public Component
{
public:

	bool Rotate = false;

	void OnCreate()
	{
		Renderer* SomeRenderer = GetApplication()->AddComponent< Renderer >( GetObject() );

		SomeTextureDiffuse = new Texture( "Resources/SoulSpear/soulspear_diffuse.tga", TextureType_Diffuse );
		SomeTextureNormal = new Texture( "Resources/SoulSpear/soulspear_normal.tga", TextureType_Normal );
		SomeTextureSpecular = new Texture( "Resources/SoulSpear/soulspear_specular.tga", TextureType_Specular );
		
		SomeShader = new Shader( "Resources/Simple.shader" );
		SomeShader->Compile();

		//SomeMesh = new Mesh( "Resources/SoulSpear/soulspear.obj" );
		SomeMesh = new Mesh( "Resources/Animation_Test.fbx" );
		SomeSkeleton = new Skeleton( "Resources/Animation_Test.fbx" );

		SomeMaterial = new Material();
		SomeMaterial->SetShader( SomeShader );
		SomeMaterial->SetTexture( SomeTextureDiffuse );
		SomeMaterial->SetTexture( SomeTextureNormal );
		SomeMaterial->SetTexture( SomeTextureSpecular );

		SomeRenderer->SetMaterial( SomeMaterial );
		SomeRenderer->SetMesh( SomeMesh );
	}

	void OnTick( float a_DeltaTime )
	{
		Transform* ThisTransform = GetApplication()->GetComponent< Transform >( GetObject() );

		if ( Rotate )
		{
			glm::quat Rotation = ThisTransform->GetRotation();
			Rotation = glm::rotate( Rotation, a_DeltaTime, { 0, 1, 0 } );
			ThisTransform->SetRotation( Rotation );
		}
	}

	void OnDestroy()
	{
		delete SomeMesh;
		delete SomeSkeleton;
		delete SomeShader;
		delete SomeMaterial;
		delete SomeTextureDiffuse;
		delete SomeTextureNormal;
		delete SomeTextureSpecular;
	}

private:

	Mesh*     SomeMesh;
	Skeleton* SomeSkeleton;
	Shader*   SomeShader;
	Material* SomeMaterial;
	Texture*  SomeTextureDiffuse;
	Texture*  SomeTextureNormal;
	Texture*  SomeTextureSpecular;
};