#include "Window.hpp"

Window* Window::s_ActiveWindow = nullptr;

Window::Window()
	: m_Window( nullptr )
{ }

Shared< Window > Window::Create( const char* a_Title, glm::uvec2 a_Size )
{
	static bool Initialized = []()
	{
		return glfwInit();
	}( );

	if ( !Initialized )
	{
		return nullptr;
	}

	GLFWwindow* NewWindowRaw = glfwCreateWindow( a_Size.x, a_Size.y, a_Title, 0, 0 );

	if ( !NewWindowRaw )
	{
		return nullptr;
	}

	Shared< Window > NewWindow( new Window );
	NewWindow->m_Window = NewWindowRaw;
	NewWindow->m_Title = a_Title;
	NewWindow->m_Size = a_Size;
	return NewWindow;
}

void Window::SetActive( Window* a_Window )
{
	glfwMakeContextCurrent( a_Window ? a_Window->m_Window : nullptr );
	s_ActiveWindow = a_Window;
}

Window* Window::GetActive()
{
	return s_ActiveWindow;
}