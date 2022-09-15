VERTEX_BEGIN:
#version 330

uniform mat4 u_PVM;

layout (location = 0) in vec4 i_Position;
//layout (location = 1) in vec4 i_Normal;
//layout (location = 2) in vec4 i_Tangent;
//layout (location = 3) in vec4 i_Bitangent;
layout (location = 4) in vec2 i_Texel;
//layout (location = 5) in vec4 i_Colour;

out vec2 o_Texel;

void main()
{
	o_Texel = i_Texel;
	gl_Position = u_PVM * i_Position;
}
VERTEX_END:

FRAGMENT_BEGIN:
#version 330

uniform mat4 u_PVM;
out vec4 FragColour;

uniform sampler2D u_TextureDiffuse;
in vec2 o_Texel;

void main()
{
	//FragColour = vec4( 0, 0, 1, 0 );

	FragColour = texture( u_TextureDiffuse, o_Texel );
	//FragColour = vec4( 1, 1, 0, 1 );
}
FRAGMENT_END: