#pragma once
#include <string>
#include <map>
#include <glm/glm.hpp>

#include "Pointer.hpp"

class GLFWwindow;

class Window
{
public:

	Window( const char* a_Title, glm::uvec2 a_Size );
	~Window();
	operator GLFWwindow* () const { return m_Window; }
	glm::uvec2 GetSize() const { return m_Size; }
	bool ShouldClose() const;
	static void SetActive( Window* a_Window );
	static Window* GetActive();
	static glm::vec2 GetMousePosition();
	static bool GetMouseButton( int a_Button );
	static bool GetKey( char a_Key );

private:

	GLFWwindow* m_Window;
	glm::uvec2  m_Size;
	std::string m_Title;

	static Window*                          s_ActiveWindow;
	static std::map< GLFWwindow*, Window* > s_Windows;
};