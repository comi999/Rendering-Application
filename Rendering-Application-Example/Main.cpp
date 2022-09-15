#include "RenderingApplication.hpp"
#include "Rendering.hpp"

int main()
{
	RenderingApplication RenderingApp( "Test!", { 1000, 1000 } );
	RenderingApp.Run();

	return 0;
}