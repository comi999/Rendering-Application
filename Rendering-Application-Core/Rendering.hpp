#pragma once
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Camera.hpp"

#define BUFFER_HANDLE_COUNT 9
#define MAX_LIGHT_COUNT 4
#define MAX_BONE_COUNT 300

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
	static glm::mat4& AddLight();
	static glm::mat4& AddBone();
	static void Begin();
	static void Draw();
	static void End();
	static void SetClearColour( glm::vec4 a_Colour );
	static void SetWindow( Window* a_Window ) { s_Window = a_Window; }
	static Window* GetWindow() { return s_Window; }
	static void SetMainCamera( Camera* a_Camera ) { s_MainCamera = a_Camera; }
	static Camera* GetMainCamera() { return s_MainCamera; }

private:
	
	static Window*                  s_Window;
	static glm::vec4                s_ClearColour;
	static std::list< DrawCall >    s_DrawCalls;
	static std::vector< glm::mat4 > s_Lights;
	static std::vector< glm::mat4 > s_Bones;
	static Camera*                  s_MainCamera;
	static const Mesh*              s_MainMesh;
	static const Material*          s_MainMaterial;
	static const Shader*            s_MainShader;

	static GLuint    s_ArrayHandle;
	static GLuint    s_BufferHandles[ BUFFER_HANDLE_COUNT ];
};