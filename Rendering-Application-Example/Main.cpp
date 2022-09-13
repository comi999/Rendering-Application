#include "RenderingApplication.hpp"

int main()
{
	auto Window = Window::Create( "Test!", { 1000, 1000 } );
	RenderingApplication RenderingApp( Window.get() );

	return 0;
}