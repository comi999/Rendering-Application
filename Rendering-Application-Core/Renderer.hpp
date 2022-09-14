#pragma once
#include "Object.hpp"

class Application;
class Mesh;

class Renderer
{
public:

	void OnCreate( Application* a_Application, Object a_Object );
	void OnTick( Application* a_Application, Object a_Object, float a_DeltaTime );
	void OnDestroy( Application* a_Application, Object a_Object );
	void Draw();
	void SetMesh( Mesh* a_Mesh ) { m_Mesh = a_Mesh; }
	Mesh* GetMesh() { return m_Mesh; }

private:

	Mesh* m_Mesh;
};