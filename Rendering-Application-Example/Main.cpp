#include "RenderingApplication.hpp"
#include "Rendering.hpp"

int main()
{
	if ( !Rendering::Init() )
	{
		Rendering::Terminate();
		return -1;
	}

	RenderingApplication RenderingApp( "Test!", { 1000, 1000 } );
	RenderingApp.Run();

	Rendering::Terminate();
	return 0;
}