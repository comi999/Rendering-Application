#include "RenderingApplication.hpp"
#include "Renderer.hpp"
#include "Rendering.hpp"

#include "DemoController.hpp"

RenderingApplication::RenderingApplication( const char* a_Window, glm::uvec2 a_Size )
	: Application( a_Window, a_Size )
{ }

void RenderingApplication::OnStart()
{
	m_Root = Create();
	AddComponent< DemoController >( m_Root );
}

void RenderingApplication::OnTick( float a_DeltaTime )
{
	
}

void RenderingApplication::OnStop()
{

}