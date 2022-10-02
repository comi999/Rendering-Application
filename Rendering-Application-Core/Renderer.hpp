#pragma once
#include "Object.hpp"
#include "Component.hpp"

class Application;
class Mesh;
class Material;

class Renderer : public Component
{
public:

	void Submit();
	void SetMesh( Mesh* a_Mesh ) { m_Mesh = a_Mesh; }
	Mesh* GetMesh() { return m_Mesh; }
	void SetMaterial( Material* a_Material ) { m_Material = a_Material; }
	Material* GetMaterial() { return m_Material; }

private:

	Mesh*     m_Mesh;
	Material* m_Material;
};