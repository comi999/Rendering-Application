#pragma once
#include <list>

#include "Mesh.hpp"
#include "Material.hpp"
#include <glm/glm.hpp>

struct DrawCall
{
	Mesh* Mesh;
	Material* Material;
	glm::mat4 Transform;
};

class Rendering
{
public:

	static bool Init();
	static void Terminate();
	static void Submit( const DrawCall& a_DrawCall );
	static void Draw();

	static void SetClearColour( glm::vec4 a_Colour );

private:
	
	static glm::vec4             s_ClearColour;
	static std::list< DrawCall > s_DrawCalls;
};