#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Pointer.hpp"

class Window
{
private:

	Window();

public:

	static Shared< Window > Create( const char* a_Title, glm::uvec2 a_Size );
	static void SetActive( Window* a_Window );
	static Window* GetActive();

private:

	GLFWwindow* m_Window;
	glm::uvec2  m_Size;
	std::string m_Title;

	static Window* s_ActiveWindow;
};