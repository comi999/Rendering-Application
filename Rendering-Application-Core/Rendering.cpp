#include "Rendering.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"

glm::vec4             Rendering::s_ClearColour;
std::list< DrawCall > Rendering::s_DrawCalls;

bool Rendering::Init()
{
	if ( !glfwInit() ) return false;
	if ( !glewInit() ) return false;
}

void Rendering::Terminate()
{
	glfwTerminate();
}

void Rendering::Submit( const DrawCall& a_DrawCall )
{
	
}

void Rendering::Draw()
{
	if ( Window* ActiveWindow = Window::GetActive() )
	{
		glClearColor( s_ClearColour.r, s_ClearColour.g, s_ClearColour.b, s_ClearColour.a );
		glClear( GL_COLOR_BUFFER_BIT );

		// Process drawing queue.
		// Code...

		glfwSwapBuffers( *Window::GetActive() );
		glfwPollEvents();
	}
}

void Rendering::SetClearColour( glm::vec4 a_Colour )
{
	s_ClearColour = a_Colour;
}