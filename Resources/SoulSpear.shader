VERTEX_BEGIN
#version 330

uniform mat4 u_PVM;
uniform mat4 u_M;

layout( location = 0 ) in vec4 i_Position;
layout( location = 1 ) in vec4 i_Normal;
layout( location = 2 ) in vec4 i_Tangent;
layout( location = 3 ) in vec4 i_Bitangent;
layout( location = 4 ) in vec2 i_Texel;
//layout ( location = 5 ) in vec4 i_Colour;
layout( location = 7 ) in uvec4 i_BoneIndices;
layout( location = 8 ) in vec4 i_BoneWeights;

out vec4 o_Position;
out vec2 o_Texel;
out vec4 o_Normal;
out vec4 o_Tangent;
out vec4 o_Bitangent;

void main()
{
	o_Position = u_M * i_Position;
	o_Texel = i_Texel;
	o_Normal = u_M * i_Normal;
	o_Tangent = u_M * i_Tangent;
	o_Bitangent = u_M * i_Bitangent;

	gl_Position = u_PVM * i_Position;
}
VERTEX_END

FRAGMENT_BEGIN
#version 330

uniform mat4 u_Lights[ 4 ];
uniform int u_LightCount;
uniform vec4 u_Colour;
uniform sampler2D u_TextureDiffuse;
uniform sampler2D u_TextureNormal;
uniform sampler2D u_TextureSpecular;

in vec4 o_Position;
in vec2 o_Texel;
in vec4 o_Normal;
in vec4 o_Tangent;
in vec4 o_Bitangent;

out vec4 FragColour;

void main()
{
	vec4 SampleDiffuse  = texture( u_TextureDiffuse, o_Texel );
	vec4 SampleNormal   = texture( u_TextureNormal, o_Texel );
	vec4 SampleSpecular = texture( u_TextureSpecular, o_Texel );

	mat3 TBN = 
		mat3( 
			normalize( vec3( o_Tangent   ) ), 
			normalize( vec3( o_Bitangent ) ), 
			normalize( vec3( o_Normal    ) ) 
		);

	vec3 SurfaceNormal = vec3( -1.0 ) + 2.0 * vec3( SampleNormal );
	SurfaceNormal = TBN * SurfaceNormal;

	vec3 Accumulated = vec3( 0.0 );

	for ( int i = 0; i < u_LightCount; ++i )
	{
		float Radius      = u_Lights[ i ][ 0 ][ 0 ];
		float Attenuation = u_Lights[ i ][ 0 ][ 1 ];
		float Distance    = u_Lights[ i ][ 0 ][ 2 ];
		float Falloff     = u_Lights[ i ][ 0 ][ 3 ];
		vec3  Colour      = vec3( u_Lights[ i ][ 1 ] );
		vec3  Position    = vec3( u_Lights[ i ][ 2 ] );
		vec3  Direction   = vec3( u_Lights[ i ][ 3 ] );
		int   Type        = int( u_Lights[ i ][ 1 ][ 3 ] );

		vec3  SurfaceToLight = Position - vec3( o_Position );
		vec3  LightToSurface = -SurfaceToLight;
		float DistanceSqrd = dot( SurfaceToLight, SurfaceToLight );
		float Angle = dot( normalize( SurfaceToLight ), SurfaceNormal );
		float Angle2 = dot( normalize( LightToSurface ), Direction );

		switch ( Type )
		{
			case 0: // Directional
			{
				break;
			}
			case 1: // Point
			{
				Accumulated += Colour / DistanceSqrd;
				break;
			}
			case 2: // Spot
			{
				break;
			}
		}
	}
	
	FragColour = SampleDiffuse* vec4( Accumulated, 1.0 );
}
FRAGMENT_END