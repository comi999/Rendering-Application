#include "Application.hpp"

Application::Application( Window* a_Window )
	: m_Window( a_Window )
{ }

Application::~Application()
{ }

void Application::Run()
{
	OnStart();

	while ( !m_ShouldClose )
	{
		// Calculate delta time
		float DeltaTime = 0.016f;


		OnTick( DeltaTime );
	}

	OnStop();
}

void Application::Quit()
{
	m_ShouldClose = true;
}