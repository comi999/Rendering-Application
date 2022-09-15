#include "Application.hpp"
#include "Rendering.hpp"
#include "Renderer.hpp"
#include "Transform.hpp"

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

	// Trigger the applications OnStart virtual function.
	OnStart();

	while ( m_Running )
	{
		// Calculate delta time
		float DeltaTime = 0.016f;
		
		// Call OnTick for all components.
		UpdateAllComponents( DeltaTime );

		// Update the applications own virtual OnTick function.
		OnTick( DeltaTime );

		// Check if the Application should. Will close on next loop.
		if ( m_Window->ShouldClose() )
		{
			Quit();
		}

		// Set the main camera.
		Rendering::SetMainCamera( GetComponent< Camera >( m_MainCamera ) );

		// Query all renderers to submit draw calls.
		PatchComponents< Renderer >( []( Renderer& a_Renderer ) { a_Renderer.Submit(); } );

		// Trigger Rendering pipeline to process DrawCalls.
		Rendering::Draw();
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
	return m_MainCamera == entt::null ? nullptr : GetComponent< Camera >( m_MainCamera );
}

void Application::SetMainCamera( Camera* a_Camera )
{
	m_MainCamera = a_Camera ? a_Camera->GetObject() : entt::null;
}

void Application::UpdateAllComponents( float a_DeltaTime )
{
	for ( auto Updater : m_Updaters )
	{
		if ( !Updater )
		{
			continue;
		}

		Updater( this, a_DeltaTime );
	}
}