#pragma once
#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>

namespace Utility
{
	static void Convert( const aiMatrix4x4& a_From, glm::mat4& a_To )
	{
		for ( uint32_t x = 0; x < 4; ++x )
		for ( uint32_t y = 0; y < 4; ++y )
		{
			a_To[ x ][ y ] = a_From[ y ][ x ];
		}
	}

	class OnStart
	{
	public:

		OnStart( void(*a_OnStart )() ) { a_OnStart(); }
	};
}