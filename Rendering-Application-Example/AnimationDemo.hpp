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
#include "Animation.hpp"
#include "Animator.hpp"
#include "ObjectRotator.hpp"

class AnimationDemo : public Component
{
public:

	void OnCreate()
	{
		//GetApplication()->GetComponent< Transform >( GetObject() )->SetScale( { 0.02f, 0.02f, 0.02f } );

		SomeTextureDiffuse = new Texture( "Resources/SoulSpear/soulspear_diffuse.tga", TextureType_Diffuse );
		//SomeTextureNormal = new Texture( "Resources/SoulSpear/soulspear_normal.tga", TextureType_Normal );
		//SomeTextureSpecular = new Texture( "Resources/SoulSpear/soulspear_specular.tga", TextureType_Specular );
		
		SomeShader = new Shader( "Resources/Animation_Test.shader" );
		SomeShader->Compile();

		SomeMesh = new Mesh( "Resources/Animation_Test.fbx" );
		//SomeSkeleton = new Skeleton( "Resources/Animation_Test.fbx" );
		SomeSkeleton = new Skeleton();
		SomeSkeleton->AddBone( "Hip", -1, glm::mat4( 1.0f ) );
		SomeSkeleton->AddBone( "Leg1", "Hip", glm::translate( glm::mat4( 1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) ) );
		SomeSkeleton->AddBone( "Leg2", "Leg1", glm::translate( glm::mat4( 1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) ) );
		SomeSkeleton->AddBone( "Leg3", "Leg2", glm::translate( glm::mat4( 1.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) ) );

		SomeAnimation = new Animation( "Resources/Animation_Test.fbx" );

		Animator* SomeAnimator = GetApplication()->AddComponent< Animator >( GetObject() );
		SomeAnimator->SetSkeleton( SomeSkeleton );
		SomeAnimator->SetAnimation( SomeAnimation );
		SomeAnimator->Repeat( true );
		SomeAnimator->SetPlaybackSpeed( 12.0f );
		SomeAnimator->Play();

		SomeMaterial = new Material();
		SomeMaterial->SetShader( SomeShader );
		SomeMaterial->SetTexture( SomeTextureDiffuse );
		//SomeMaterial->SetTexture( SomeTextureNormal );
		//SomeMaterial->SetTexture( SomeTextureSpecular );
		
		Renderer* SomeRenderer = GetApplication()->AddComponent< Renderer >( GetObject() );
		SomeRenderer->SetMaterial( SomeMaterial );
		SomeRenderer->SetMesh( SomeMesh );
	}

	void OnDestroy()
	{
		delete SomeMesh;
		delete SomeSkeleton;
		delete SomeAnimation;
		delete SomeShader;
		delete SomeMaterial;
		delete SomeTextureDiffuse;
		delete SomeTextureNormal;
		delete SomeTextureSpecular;
	}

private:

	Mesh*      SomeMesh;
	Skeleton*  SomeSkeleton;
	Animation* SomeAnimation;
	Shader*    SomeShader;
	Material*  SomeMaterial;
	Texture*   SomeTextureDiffuse;
	Texture*   SomeTextureNormal;
	Texture*   SomeTextureSpecular;
};