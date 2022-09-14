#include "Application.hpp"
#include "Rendering.hpp"
#include "Renderer.hpp"

Application::Application( const char* a_Title, glm::uvec2 a_Size )
	: m_Running( false )
	, m_Window( a_Title, a_Size )
{ }

Application::~Application()
{ }

bool Application::Running() const
{
	return m_Running;
}

void Application::Run()
{
	m_Running = true;
	Rendering::Init();
	OnStart();

	while ( m_Running )
	{
		// Calculate delta time
		float DeltaTime = 0.016f;
		
		UpdateAllComponents( DeltaTime );
		OnTick( DeltaTime );

		if ( m_Window.ShouldClose() )
		{
			Quit();
		}

		Window::SetActive( &m_Window );
		Rendering::Draw();
	}

	OnStop();
}

void Application::Quit()
{
	m_Running = false;
}

Object Application::Create()
{
	return m_Registry.create();
}

void Application::Destroy( Object a_Object )
{
	m_Registry.destroy( a_Object );
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