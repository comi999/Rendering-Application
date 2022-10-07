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
#include "LineRenderer.hpp"

class AnimationDemo : public Component
{
public:

	void OnCreate()
	{
		GetApplication()->GetComponent< Transform >( GetObject() )->SetScale( glm::vec3{ 0.02f, 0.02f, 0.02f } );

		SomeTextureDiffuse = new Texture( "Resources/SoulSpear/soulspear_diffuse.tga", TextureType_Diffuse );

		SomeShader = new Shader( "Resources/Animation_Test.shader" );
		SomeShader->Compile();
		
		/*SomeMesh = new Mesh( "Resources/Animation_Test.fbx" );
		SomeSkeleton = new Skeleton( "Resources/Animation_Test.fbx" );
		SomeAnimation = new Animation( "Resources/Animation_Test.fbx" );*/

		SomeMesh = new Mesh( "Resources/Taunt2.fbx" );
		SomeSkeleton = new Skeleton( "Resources/Taunt2.fbx" );
		SomeAnimation = new Animation( "Resources/Taunt2.fbx" );
		/*SomeMesh = new Mesh( "Resources/AnimTest.fbx" );
		SomeSkeleton = new Skeleton( "Resources/AnimTest.fbx" );
		SomeAnimation = new Animation( "Resources/AnimTest.fbx" );*/

		Animator* SomeAnimator = GetApplication()->AddComponent< Animator >( GetObject() );
		SomeAnimator->SetSkeleton( SomeSkeleton );
		SomeAnimator->SetAnimation( SomeAnimation );
		SomeAnimator->Repeat( true );
		SomeAnimator->SetPlaybackSpeed( 12.0f );
		SomeAnimator->Play();
		SomeAnimator->DebugEnabled( false );

		SomeMaterial = new Material();
		SomeMaterial->SetShader( SomeShader );
		SomeMaterial->SetTexture( SomeTextureDiffuse );

		Renderer* SomeRenderer = GetApplication()->AddComponent< Renderer >( GetObject() );
		SomeRenderer->SetMaterial( SomeMaterial );
		SomeRenderer->SetMesh( SomeMesh );
		SomeRenderer->SetEnabled( true );
	}

	void OnDestroy()
	{
		delete SomeMesh;
		delete SomeSkeleton;
		delete SomeAnimation;
		delete SomeShader;
		delete SomeMaterial;
		delete SomeTextureDiffuse;
	}

private:

	Mesh*      SomeMesh;
	Skeleton*  SomeSkeleton;
	Animation* SomeAnimation;
	Shader*    SomeShader;
	Material*  SomeMaterial;
	Texture*   SomeTextureDiffuse;
};