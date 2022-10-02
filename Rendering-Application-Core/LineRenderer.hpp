#pragma once
#include <glm/glm.hpp>

#include "Component.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Utilities.hpp"

class LineRenderer : public Component
{
public:

	void SetStartEnd( const glm::vec3& a_Start, const glm::vec3& a_End );
	void OnRender();

};