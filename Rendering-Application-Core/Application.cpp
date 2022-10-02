#include "Application.hpp"
#include "Rendering.hpp"
#include "Renderer.hpp"
#include "LineRenderer.hpp"
#include "Transform.hpp"
#include "Light.hpp"
#include "Animator.hpp"
#include "Input.hpp"


std::vector< Application::OnTickFunction   > Application::s_OnTickFunctions;
std::vector< Application::OnRenderFunction > Application::s_OnRenderFunctions;

Application::Application( const char* a_Title, glm::uvec2 a_Size )
	: m_Running( false )
	, m_Window( new Window( a_Title, a_Size ) )
{ }

Application::~Application()
{
	delete m_Window;
}

bool Application::Running() const
{
	return m_Running;
}

void Application::Run()
{
	m_Running = true;

	// Initialize rendering pipeline.
	Rendering::SetWindow( m_Window );
	Rendering::Init();
	Input::Init();

	// Trigger the applications OnStart virtual function.
	OnStart();

	while ( m_Running )
	{
		// Calculate delta time
		float DeltaTime = 0.016f;

		Input::Tick();
		
		// Call OnTick for all components.
		OnTick( DeltaTime );

		// Check if the Application should. Will close on next loop.
		if ( m_Window->ShouldClose() )
		{
			Quit();
		}

		// Set the main camera.
		Rendering::SetMainCamera( GetComponent< Camera >( m_MainCamera ) );

		// Setup rendering for frame.
		Rendering::Begin();

		PatchComponents< Transform >( []( Transform& a_Transform ) { if ( a_Transform.IsEnabled() ) a_Transform.Update(); } );

		// Run OnRender for all components.
		OnRender();

		// Query all renderers to submit draw calls.
		//PatchComponents< Renderer >( []( Renderer& a_Renderer ) { if ( a_Renderer.IsEnabled() ) a_Renderer.Submit(); } );

		// Query all line renderers to submit draw calls.
		//PatchComponents< LineRenderer >( []( LineRenderer& a_LineRenderer ) { if ( a_LineRenderer.IsEnabled() ) a_LineRenderer.Submit(); } );

		// Query all lights to submit.
		PatchComponents< Light >( []( Light& a_Light ) { if ( a_Light.IsEnabled() ) a_Light.BuildMatrix( Rendering::AddLight() ); } );

		// Query all Animator's to submit bones.
		// TO DO: This needs to be part of some sort of stage for all components. And this is adding
		// bones to a list that is not animator specific which will become a problem if I have more animators.
		PatchComponents< Animator >( []( Animator& a_Animator ) 
		{
			if ( !a_Animator.IsEnabled() ) return;

			for ( uint32_t i = 1; i < a_Animator.GetSkeleton()->GetBoneCount(); ++i )
			{
				a_Animator.BuildMatrix( Rendering::AddBone(), i );
			}
		} );

		// Trigger Rendering pipeline to process DrawCalls.
		Rendering::Draw();

		// Flush Rendering
		Rendering::End();
	}

	// Trigger the Applications OnStop virtual function.
	OnStop();

	// Terminate rendering.
	Rendering::Terminate();
}

void Application::Quit()
{
	m_Running = false;
}

Object Application::Create()
{
	Object New = m_Registry.create();
	AddComponent< Transform >( New );
	return New;
}

void Application::Destroy( Object a_Object )
{
	m_Registry.destroy( a_Object );
}

Camera* Application::GetMainCamera()
{
	return m_MainCamera == NullObject ? nullptr : GetComponent< Camera >( m_MainCamera );
}

void Application::SetMainCamera( Camera* a_Camera )
{
	m_MainCamera = a_Camera ? a_Camera->GetObject() : NullObject;
}

void Application::OnTick( float a_DeltaTime )
{
	for ( auto OnTickFunction : s_OnTickFunctions )
	{
		if ( !OnTickFunction )
		{
			continue;
		}

		OnTickFunction( this, a_DeltaTime );
	}
}

void Application::OnRender()
{
	for ( auto OnRenderFunction : s_OnRenderFunctions )
	{
		if ( !OnRenderFunction )
		{
			continue;
		}

		OnRenderFunction( this );
	}
}