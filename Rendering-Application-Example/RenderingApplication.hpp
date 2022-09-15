#pragma once

#include "Application.hpp"

class RenderingApplication : public Application
{
public:

	RenderingApplication( const char* a_Title, glm::uvec2 a_Size );

protected:

	void OnStart() override;
	void OnTick( float a_DeltaTime ) override;
	void OnStop() override;

private:

	Object m_Root;
	Object m_CameraObject;
};