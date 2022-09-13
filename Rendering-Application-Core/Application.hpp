#pragma once
#include "Window.hpp"

class Application
{
public:

	Application( Window* a_Window );
	virtual ~Application();
	void Run();
	void Quit();

protected:

	virtual void OnStart() = 0;
	virtual void OnTick( float a_DeltaTime ) = 0;
	virtual void OnStop() = 0;

private:

	Window* m_Window;
	bool    m_ShouldClose;
};