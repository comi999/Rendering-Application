#include "RenderingApplication.hpp"
#include "Renderer.hpp"
#include "Rendering.hpp"

// Delete me
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "DemoController.hpp"

Mesh* SomeMesh;
Shader* SomeShader;
Material* SomeMaterial;
Texture* SomeTexture;

RenderingApplication::RenderingApplication( const char* a_Window, glm::uvec2 a_Size )
	: Application( a_Window, a_Size )
{ }

void RenderingApplication::OnStart()
{
	SomeShader = new Shader( "Resources/Simple.shader" );
	SomeShader->Compile();

	SomeMesh = new Mesh( "Resources/SoulSpear/soulspear.obj" );

	m_Root = Create();
	Renderer* SomeRenderer = AddComponent< Renderer >( m_Root );
	SomeRenderer->SetMesh( SomeMesh );
	AddComponent< DemoController >( m_Root );

	m_CameraObject = Create();
	Camera* SomeCamera = AddComponent< Camera >( m_CameraObject );
	SetMainCamera( SomeCamera );
	Transform* CameraTransform = GetComponent< Transform >( m_CameraObject );
	CameraTransform->SetPosition( { 0.0f, 2.0f, -5.0f } );

	SomeTexture = new Texture( "Resources/SoulSpear/soulspear_diffuse.png" );

	SomeMaterial = new Material();
	SomeMaterial->SetShader( SomeShader );
	SomeMaterial->SetDiffuse( SomeTexture );

	SomeRenderer->SetMaterial( SomeMaterial );
}

void RenderingApplication::OnTick( float a_DeltaTime )
{
	
}

void RenderingApplication::OnStop()
{

}