#pragma once

#include "Application.hpp"

class RenderingApplication : public Application
{
public:

	RenderingApplication( Window* a_Window );

protected:

	void OnStart() override;
	void OnTick( float a_DeltaTime ) override;
	void OnStop() override;

};