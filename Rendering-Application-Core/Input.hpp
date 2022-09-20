#pragma once
#include <glm/glm.hpp>

class Input
{
public:

	static void Init();
	static void Tick();
	static bool MouseButtonPressed( int a_Button );
	static bool MouseButtonReleased( int a_Button );
	static bool MouseButtonDown( int a_Button );
	static bool KeyPressed( char a_Key );
	static bool KeyReleased( char a_Key );
	static bool KeyDown( char a_Key );
	static glm::vec2 GetMouseDelta();
	static glm::vec2 GetMousePosition();

private:

	static bool s_LastStateKey[ 26 ];
	static bool s_NowStateKey[ 26 ];
	static bool s_LastStateMouseButton[ 2 ];
	static bool s_NowStateMouseButton[ 2 ];
	static glm::vec2 s_LastStateMousePosition;
	static glm::vec2 s_NowStateMousePosition;
};