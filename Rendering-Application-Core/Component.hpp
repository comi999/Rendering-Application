#pragma once
#include "Object.hpp"

class Application;

class Component
{
public:

	Object GetObject() const { return m_Object; }
	Application* GetApplication() const { return m_Application; }

private:

	friend class Application;

	Object       m_Object;
	Application* m_Application;
};