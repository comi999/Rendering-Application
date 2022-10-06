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

	glm::vec3 Start;
	glm::vec3 End;

	LineRenderer();
	void OnRender();

};