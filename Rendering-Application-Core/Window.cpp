#include "Window.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Window*                          Window::s_ActiveWindow = nullptr;
std::map< GLFWwindow*, Window* > Window::s_Windows;

Window::Window( const char* a_Title, glm::uvec2 a_Size )
{
	static bool GLFWInitialized = glfwInit();
	m_Window = glfwCreateWindow( a_Size.x, a_Size.y, a_Title, nullptr, nullptr );
	if ( !m_Window ) return;
	s_Windows[ m_Window ] = this;
	m_Size = a_Size;
}

Window::~Window()
{
	if ( !m_Window ) return;
	auto Iter = s_Windows.find( m_Window );
	if ( Iter != s_Windows.end() ) s_Windows.erase( Iter );
	glfwDestroyWindow( m_Window );
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

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose( m_Window );
}