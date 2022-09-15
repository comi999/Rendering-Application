#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

class Mesh
{
public:

	Mesh() = default;
	Mesh( const std::string& a_Path );
	uint32_t         GetIndexCount() const { return m_Indices.size(); }
	uint32_t         GetVertexCount() const { return m_Positions.size(); }
	const uint32_t*  GetIndices() const { return m_Indices.data(); }
	const glm::vec4* GetPositions() const { return m_Positions.data(); }
	const glm::vec4* GetNormals() const { return m_Normals.data(); }
	const glm::vec4* GetTangents() const { return m_Tangents.data(); }
	const glm::vec4* GetBitangents() const { return m_Bitangents.data(); }
	const glm::vec2* GetTexels() const { return m_Texels.data(); }
	const glm::vec4* GetColours() const { return m_Colours.data(); }

private:

	std::vector< uint32_t >  m_Indices;
	std::vector< glm::vec4 > m_Positions;
	std::vector< glm::vec4 > m_Normals;
	std::vector< glm::vec4 > m_Tangents;
	std::vector< glm::vec4 > m_Bitangents;
	std::vector< glm::vec2 > m_Texels;
	std::vector< glm::vec4 > m_Colours;
};