#include "LineRenderer.hpp"
#include "Rendering.hpp"
#include "Application.hpp"
#include "Transform.hpp"
#include "Utilities.hpp"

void LineRenderer::SetStartEnd( const glm::vec3& a_Start, const glm::vec3& a_End )
{
	float Scale = glm::distance( a_Start, a_End );
	glm::quat Rotation = glm::quatLookAt( glm::normalize( a_End - a_Start ), { 0.0f, 1.0f, 0.0f } );

	Transform* ThisTransform = GetApplication()->GetComponent< Transform >( GetObject() );
	ThisTransform->SetPosition( a_Start );
	ThisTransform->SetRotation( Rotation );
	ThisTransform->SetScale( { Scale, 1.0f, 1.0f } );
}

void LineRenderer::OnRender()
{
	// This is a crap way to do this, this should be a resource
	static Mesh s_LineMesh;
	static Material s_LineMaterial;
	static Shader s_LineShader;
	static Utility::OnStart s_OnStart = []()
	{
		s_LineMesh.AddPosition( { 10.0f, 0.0f, 0.0f, 1.0f } );
		s_LineMesh.AddPosition( { 10.0f, 1.0f, 0.0f, 1.0f } );
		s_LineMesh.AddPosition( { 0.0f, 1.0f, 0.0f, 1.0f } );
		s_LineMesh.AddPosition( { 0.0f, 0.0f, 0.0f, 1.0f } );
		s_LineMesh.AddColour( { 1.0f, 0.0f, 0.0f, 1.0f } );
		s_LineMesh.AddColour( { 1.0f, 0.0f, 0.0f, 1.0f } );
		s_LineMesh.AddColour( { 0.0f, 0.0f, 1.0f, 1.0f } );
		s_LineMesh.AddColour( { 0.0f, 0.0f, 1.0f, 1.0f } );
		s_LineMesh.AddIndex( 0 );
		s_LineMesh.AddIndex( 1 );
		s_LineMesh.AddIndex( 2 );
		s_LineMesh.AddIndex( 0 );
		s_LineMesh.AddIndex( 2 );
		s_LineMesh.AddIndex( 3 );

		s_LineShader = Shader( "Resources/LineRenderer.shader" );
		s_LineShader.Compile();

		s_LineMaterial.SetShader( &s_LineShader );
	};

	Application* ThisApplication = GetApplication();
	Transform* ThisTransform = ThisApplication->GetComponent< Transform >( GetObject() );
	DrawCall ToSubmit;
	ToSubmit.Mesh = &s_LineMesh;
	ToSubmit.Material = &s_LineMaterial;
	ToSubmit.Transform = ThisTransform->GetGlobal();

	Rendering::Submit( ToSubmit );
}