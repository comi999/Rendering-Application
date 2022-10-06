#include "LineRenderer.hpp"
#include "Rendering.hpp"
#include "Application.hpp"
#include "Transform.hpp"
#include "Utilities.hpp"

LineRenderer::LineRenderer()
	: Start( 0.0f )
	, End( 0.0f )
{ }

void LineRenderer::OnRender()
{
	// This is a crap way to do this, this should be a resource
	static Mesh s_LineMesh;
	static Material s_LineMaterial;
	static Shader s_LineShader;
	static Utility::OnStart s_OnStart = []()
	{
		s_LineMesh.AddPosition( { 0.0f, 0.0f, 0.0f, 1.0f } );
		s_LineMesh.AddPosition( { 0.0f, 0.0f, 0.0f, 1.0f } );
		s_LineMesh.AddColour( { 1.0f, 0.0f, 0.0f, 1.0f } );
		s_LineMesh.AddColour( { 0.0f, 1.0f, 0.0f, 1.0f } );
		s_LineMesh.AddIndex( 0 );
		s_LineMesh.AddIndex( 1 );

		s_LineShader = Shader( "Resources/LineRenderer.shader" );
		s_LineShader.Compile();

		s_LineMaterial.SetShader( &s_LineShader );
	};

	s_LineMesh.GetPosition( 0 ) = { Start, 1.0f };
	s_LineMesh.GetPosition( 1 ) = { End, 1.0f };

	DrawCall ToSubmit;
	ToSubmit.Mesh = &s_LineMesh;
	ToSubmit.Material = &s_LineMaterial;
	ToSubmit.RenderingMode = RenderingMode_Line;
	ToSubmit.Transform = GetApplication()->GetComponent< Transform >( GetObject() )->GetGlobal();
	Rendering::Submit( ToSubmit );
}