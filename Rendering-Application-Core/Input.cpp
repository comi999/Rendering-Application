#include "Input.hpp"
#include "Window.hpp"

bool      Input::s_LastStateKey[ 26 ] { false };
bool      Input::s_NowStateKey[ 26 ] { false };
bool      Input::s_LastStateMouseButton[ 2 ] { false };
bool      Input::s_NowStateMouseButton[ 2 ] { false };
glm::vec2 Input::s_LastStateMousePosition;
glm::vec2 Input::s_NowStateMousePosition;

void Input::Init()
{
	s_LastStateMousePosition = Window::GetMousePosition();
	s_NowStateMousePosition = s_LastStateMousePosition;
}

void Input::Tick()
{
	if ( !Window::GetActive() )
	{
		return;
	}

	for ( uint32_t i = 0; i < 26; ++i )
	{
		s_LastStateKey[ i ] = s_NowStateKey[ i ];
		s_NowStateKey[ i ] = Window::GetKey( 'A' + i );
	}

	for ( uint32_t i = 0; i < 2; ++i )
	{
		s_LastStateMouseButton[ i ] = s_NowStateMouseButton[ i ];
		s_NowStateMouseButton[ i ] = Window::GetMouseButton( i );
	}

	s_LastStateMousePosition = s_NowStateMousePosition;
	s_NowStateMousePosition = Window::GetMousePosition();
}

bool Input::MouseButtonPressed( int a_Button )
{
	return !s_LastStateMouseButton[ a_Button ] && s_NowStateMouseButton[ a_Button ];
}

bool Input::MouseButtonReleased( int a_Button )
{
	return s_LastStateMouseButton[ a_Button ] && !s_NowStateMouseButton[ a_Button ];
}

bool Input::MouseButtonDown( int a_Button )
{
	return s_NowStateMouseButton[ a_Button ];
}

bool Input::KeyPressed( char a_Key )
{
	return !s_LastStateKey[ a_Key - 'A' ] && s_NowStateKey[ a_Key - 'A' ];
}

bool Input::KeyReleased( char a_Key )
{
	return s_LastStateKey[ a_Key - 'A' ] && !s_NowStateKey[ a_Key - 'A' ];
}

bool Input::KeyDown( char a_Key )
{
	return s_NowStateKey[ a_Key - 'A' ];
}

glm::vec2 Input::GetMouseDelta()
{
	return s_NowStateMousePosition - s_LastStateMousePosition;
}

glm::vec2 Input::GetMousePosition()
{
	return s_NowStateMousePosition;
}
