#include "Shader.hpp"
#include <regex>
#include <iostream>

Shader::Shader( const std::string& a_Path )
	: m_ProgramHandle( 0 )
	, m_UniformLocationPVM( -1 )
	, m_UniformLocationM( -1 )
{
	FILE* SourceFile = nullptr;

	if ( fopen_s( &SourceFile, a_Path.c_str(), "r+t" ) == 0 )
	{
		// Attempt to read in 25 characters
		fseek( SourceFile, 0, SEEK_END );
		size_t Size = ftell( SourceFile );
		fseek( SourceFile, 0, SEEK_SET );
		m_Source.resize( Size );
		fread( m_Source.data(), sizeof( std::string::value_type ), Size, SourceFile );
		fclose( SourceFile );
	}
}

bool Shader::Compile()
{
	if ( m_ProgramHandle )
	{
		return false;
	}

	std::regex Regex( "(VERTEX_BEGIN|VERTEX_END|FRAGMENT_BEGIN|FRAGMENT_END)" );
	//std::regex Regex( "/VERTEX_BEGIN.*VERTEX_END/" );
	std::sregex_iterator Begin( m_Source.begin(), m_Source.end(), Regex ), End;
	size_t VertexBegin, VertexEnd, FragmentBegin, FragmentEnd;
	bool VertexBeginFound = false, VertexEndFound = false, FragmentBeginFound = false, FragmentEndFound = false;

	for ( ; Begin != End; ++Begin )
	{
		std::string Match = Begin->str();

		if ( Match == "VERTEX_BEGIN" )
		{
			VertexBegin = Begin->position() + Begin->length();
			VertexBeginFound = true;
		}
		else if ( Match == "VERTEX_END" )
		{
			VertexEnd = Begin->position();
			VertexEndFound = true;
		}
		else if ( Match == "FRAGMENT_BEGIN" )
		{
			FragmentBegin = Begin->position() + Begin->length();
			FragmentBeginFound = true;
		}
		else if ( Match == "FRAGMENT_END" )
		{
			FragmentEnd = Begin->position();
			FragmentEndFound = true;
		}
	}

	if ( !VertexBeginFound || !VertexEndFound || !FragmentBeginFound || !FragmentEndFound )
	{
		return false;
	}

	std::string VertexSource( m_Source.begin() + VertexBegin, m_Source.begin() + VertexEnd );
	std::string FragmentSource( m_Source.begin() + FragmentBegin, m_Source.begin() + FragmentEnd );
	const char* Sources[] = { VertexSource.c_str(), FragmentSource.c_str() };
	GLint Lengths[] = { VertexSource.length(), FragmentSource.length() };

	auto VertexShader = glCreateShader( GL_VERTEX_SHADER );
	auto FragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( VertexShader, 1, Sources + 0, Lengths + 0 );
	glShaderSource( FragmentShader, 1, Sources + 1, Lengths + 1 );
	glCompileShader( VertexShader );
	glCompileShader( FragmentShader );
	m_ProgramHandle = glCreateProgram();
	glAttachShader( m_ProgramHandle, VertexShader );
	glAttachShader( m_ProgramHandle, FragmentShader );
	glLinkProgram( m_ProgramHandle );
	glDeleteShader( VertexShader );
	glDeleteShader( FragmentShader );

	char info[ 1000 ];
	GLsizei siz;
	glGetProgramInfoLog( m_ProgramHandle, 1000, &siz, info );
	std::cout << info << std::endl;

	// Find locations
	m_UniformLocationColour = glGetUniformLocation( m_ProgramHandle, "u_Colour" );
	m_UniformLocationPVM = glGetUniformLocation( m_ProgramHandle, "u_PVM" );
	m_UniformLocationM = glGetUniformLocation( m_ProgramHandle, "u_M" );

	m_UniformLocationTextures[ TextureType_Diffuse ] = glGetUniformLocation( m_ProgramHandle, "u_TextureDiffuse" );
	m_UniformLocationTextures[ TextureType_Normal ] = glGetUniformLocation( m_ProgramHandle, "u_TextureNormal" );
	m_UniformLocationTextures[ TextureType_Specular ] = glGetUniformLocation( m_ProgramHandle, "u_TextureSpecular" );

	m_UniformLocationLights = glGetUniformLocation( m_ProgramHandle, "u_Lights" );
	m_UniformLocationLightCount = glGetUniformLocation( m_ProgramHandle, "u_LightCount" );

	m_UniformLocationBones = glGetUniformLocation( m_ProgramHandle, "u_Bones" );
	m_UniformLocationBoneCount = glGetUniformLocation( m_ProgramHandle, "u_BoneCount" );

	return true;
}

bool Shader::Decompile()
{
	if ( !m_ProgramHandle )
	{
		return false;
	}

	glDeleteProgram( m_ProgramHandle );
	m_ProgramHandle = 0;
	return true;
}

bool Shader::Use() const
{
	if ( !m_ProgramHandle )
	{
		return false;
	}

	glUseProgram( m_ProgramHandle );
	return true;
}

bool Shader::SetLights( const glm::mat4* a_Lights, uint32_t a_LightCount ) const
{
	if ( m_UniformLocationLights == -1 || m_UniformLocationLightCount == -1 )
	{
		//return false; // Because I'm not using LightCount in the shader.
	}

	glUniform1i( m_UniformLocationLightCount, a_LightCount );
	glUniformMatrix4fv( m_UniformLocationLights, a_LightCount, false, &a_Lights[ 0 ][ 0 ][ 0 ] );
	return true;
}

bool Shader::SetBones( const glm::mat4* a_Bones, uint32_t a_BoneCount ) const
{
	if ( m_UniformLocationBones == -1 || m_UniformLocationBoneCount == -1 )
	{
		//return false;
	}

	glUniform1i( m_UniformLocationBoneCount, a_BoneCount );
	glUniformMatrix4fv( m_UniformLocationBones, a_BoneCount, false, &a_Bones[ 0 ][ 0 ][ 0 ] );
	return true;
}

bool Shader::SetColour( const glm::vec4& a_Colour ) const
{
	if ( m_UniformLocationColour == -1 )
	{
		return false;
	}

	glUniform4f( m_UniformLocationColour, a_Colour.r, a_Colour.g, a_Colour.b, a_Colour.a );
	return true;
}

bool Shader::SetTexture( const Texture* a_Texture ) const
{
	if ( !a_Texture )
	{
		return false;
	}

	auto Type = a_Texture->GetType();

	if ( Type == TextureType_None )
	{
		return false;
	}

	auto Location = m_UniformLocationTextures[ Type ];

	if ( Location == -1 )
	{
		return false;
	}
	
	glUniform1i( Location, Type );
	glActiveTexture( GL_TEXTURE0 + Type );
	glBindTexture( GL_TEXTURE_2D, a_Texture->GetHandle() );
	return true;
}

bool Shader::SetPVM( const glm::mat4& a_Matrix ) const
{
	if ( m_UniformLocationPVM == -1 )
	{
		return false;
	}

	glUniformMatrix4fv( m_UniformLocationPVM, 1, false, &a_Matrix[ 0 ][ 0 ] );
	return true;
}

bool Shader::SetM( const glm::mat4& a_Matrix ) const
{
	if ( m_UniformLocationM == -1 )
	{
		return false;
	}

	glUniformMatrix4fv( m_UniformLocationM, 1, false, &a_Matrix[ 0 ][ 0 ] );
	return true;
}