#include "Rendering.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Camera.hpp"
#include "Material.hpp"

Window*                  Rendering::s_Window = nullptr;
glm::vec4                Rendering::s_ClearColour;
std::list< DrawCall >    Rendering::s_DrawCalls;
std::vector< glm::mat4 > Rendering::s_Lights;
Camera*                  Rendering::s_MainCamera = nullptr;
const Mesh*              Rendering::s_MainMesh = nullptr;
const Material*          Rendering::s_MainMaterial = nullptr;
const Shader*            Rendering::s_MainShader = nullptr;
GLuint                   Rendering::s_ArrayHandle;
GLuint                   Rendering::s_BufferHandles[ 7 ];

bool Rendering::Init()
{
	Window::SetActive( s_Window );
	if ( glewInit() != GLEW_OK ) return false;

	glGenVertexArrays( 1, &s_ArrayHandle );
	glGenBuffers( 7, s_BufferHandles );
}

void Rendering::Terminate()
{
	glfwTerminate();
}

void Rendering::Submit( const DrawCall& a_DrawCall )
{
	s_DrawCalls.push_back( a_DrawCall );
}

glm::mat4& Rendering::AddLight()
{
	s_Lights.emplace_back();
	return s_Lights.back();
}

void Rendering::Begin()
{
	glEnable( GL_DEPTH_TEST );
	glClearColor( s_ClearColour.r, s_ClearColour.g, s_ClearColour.b, s_ClearColour.a );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Rendering::Draw()
{
	if ( Window* ActiveWindow = Window::GetActive() )
	{
		if ( s_MainCamera )
		{
			// Get the PV matrix from the main camera.
			glm::mat4 PV = s_MainCamera->GetProjectionView();

			// Process drawing queue.
			// Code...
			struct
			{
				bool operator()( const DrawCall& a_Left, const DrawCall& a_Right )
				{
					//return a_Left.Mesh < a_Right.Mesh || a_Left.Material < a_Right.Material;

					return true;
				};
			} Sorter;
			//s_DrawCalls.sort( Sorter );

			s_MainMesh = nullptr;
			s_MainMaterial = nullptr;

			while ( !s_DrawCalls.empty() )
			{
				DrawCall ThisCall = s_DrawCalls.front();
				s_DrawCalls.pop_front();

				if ( ThisCall.Mesh != s_MainMesh )
				{
					s_MainMesh = ThisCall.Mesh;

					// Set up Mesh
					if ( !s_MainMesh )
					{
						continue;
					}

					glBindVertexArray( s_ArrayHandle );

					if ( s_MainMesh->GetPositions() )
					{
						glBindBuffer( GL_ARRAY_BUFFER, s_BufferHandles[ 0 ] );
						glBufferData( GL_ARRAY_BUFFER, s_MainMesh->GetVertexCount() * sizeof( glm::vec4 ), s_MainMesh->GetPositions(), GL_STATIC_DRAW );
						glVertexAttribPointer( 0, 4, GL_FLOAT, false, sizeof( glm::vec4 ), ( void* )0 );
						glEnableVertexAttribArray( 0 );
					} else glDisableVertexAttribArray( 0 );

					if ( s_MainMesh->GetNormals() )
					{
						glBindBuffer( GL_ARRAY_BUFFER, s_BufferHandles[ 1 ] );
						glBufferData( GL_ARRAY_BUFFER, s_MainMesh->GetVertexCount() * sizeof( glm::vec4 ), s_MainMesh->GetNormals(), GL_STATIC_DRAW );
						glVertexAttribPointer( 1, 4, GL_FLOAT, false, sizeof( glm::vec4 ), ( void* )0 );
						glEnableVertexAttribArray( 1 );
					} else glDisableVertexAttribArray( 1 );

					if ( s_MainMesh->GetTangents() )
					{
						glBindBuffer( GL_ARRAY_BUFFER, s_BufferHandles[ 2 ] );
						glBufferData( GL_ARRAY_BUFFER, s_MainMesh->GetVertexCount() * sizeof( glm::vec4 ), s_MainMesh->GetTangents(), GL_STATIC_DRAW );
						glVertexAttribPointer( 2, 4, GL_FLOAT, false, sizeof( glm::vec4 ), ( void* )0 );
						glEnableVertexAttribArray( 2 );
					} else glDisableVertexAttribArray( 2 );

					if ( s_MainMesh->GetBitangents() )
					{
						glBindBuffer( GL_ARRAY_BUFFER, s_BufferHandles[ 3 ] );
						glBufferData( GL_ARRAY_BUFFER, s_MainMesh->GetVertexCount() * sizeof( glm::vec4 ), s_MainMesh->GetBitangents(), GL_STATIC_DRAW );
						glVertexAttribPointer( 3, 4, GL_FLOAT, false, sizeof( glm::vec4 ), ( void* )0 );
						glEnableVertexAttribArray( 3 );
					} else glDisableVertexAttribArray( 3 );

					if ( s_MainMesh->GetTexels() )
					{
						glBindBuffer( GL_ARRAY_BUFFER, s_BufferHandles[ 4 ] );
						glBufferData( GL_ARRAY_BUFFER, s_MainMesh->GetVertexCount() * sizeof( glm::vec2 ), s_MainMesh->GetTexels(), GL_STATIC_DRAW );
						glVertexAttribPointer( 4, 2, GL_FLOAT, false, sizeof( glm::vec2 ), ( void* )0 );
						glEnableVertexAttribArray( 4 );
					} else glDisableVertexAttribArray( 4 );

					if ( s_MainMesh->GetColours() )
					{
						glBindBuffer( GL_ARRAY_BUFFER, s_BufferHandles[ 5 ] );
						glBufferData( GL_ARRAY_BUFFER, s_MainMesh->GetVertexCount() * sizeof( glm::vec4 ), s_MainMesh->GetColours(), GL_STATIC_DRAW );
						glVertexAttribPointer( 5, 4, GL_FLOAT, false, sizeof( glm::vec4 ), ( void* )0 );
						glEnableVertexAttribArray( 5 );
					} else glDisableVertexAttribArray( 5 );

					if ( s_MainMesh->GetIndices() )
					{
						glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, s_BufferHandles[ 6 ] );
						glBufferData( GL_ELEMENT_ARRAY_BUFFER, s_MainMesh->GetIndexCount() * sizeof( uint32_t ), s_MainMesh->GetIndices(), GL_STATIC_DRAW );
					}

					glBindBuffer( GL_ARRAY_BUFFER, 0 );
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
					glBindVertexArray( 0 );
				}

				if ( ThisCall.Material != s_MainMaterial )
				{
					// Set up Material
					s_MainMaterial = ThisCall.Material;
					s_MainShader = s_MainMaterial->GetShader();

					s_MainMaterial->Apply();
				}

				// Set up Transformation
				if ( s_MainShader )
				{
					// Set world information.
					s_MainShader->SetPVM( PV * ThisCall.Transform );
					s_MainShader->SetM( ThisCall.Transform );

					// Set light information.
					s_MainShader->SetLights( s_Lights.data(), s_Lights.size() );
				}

				// glDrawElements
				if ( s_MainMesh && s_MainMesh->GetIndices() )
				{
					glBindVertexArray( s_ArrayHandle );
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, s_BufferHandles[ 6 ] );
					glDrawElements( GL_TRIANGLES, s_MainMesh->GetIndexCount(), GL_UNSIGNED_INT, ( void* )0 );
					glBindVertexArray( 0 );
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
					glUseProgram( 0 );
				}

				// glDrawArray
				else if ( s_MainMesh )
				{
					glBindVertexArray( s_ArrayHandle );
					glDrawArrays( GL_TRIANGLES, 0, s_MainMesh->GetVertexCount() );
					glBindVertexArray( 0 );
					glUseProgram( 0 );
				}
			}
		}
	}
}

void Rendering::End()
{
	s_Lights.clear();
	glfwSwapBuffers( *Window::GetActive() );
	glfwPollEvents();
}

void Rendering::SetClearColour( glm::vec4 a_Colour )
{
	s_ClearColour = a_Colour;
}