#pragma once
#include <glm/glm.hpp>

#include "Component.hpp"

class Camera : public Component
{
public:

	Camera();
	inline void SetFOV( float a_FOV ) { m_FOV = a_FOV; m_Dirty = true; }
	inline void SetAspect( float a_Aspect ) { m_Aspect = a_Aspect; m_Dirty = true; }
	inline void SetNearZ( float a_NearZ ) { m_NearZ = a_NearZ;m_Dirty = true; }
	inline void SetFarZ( float a_FarZ ) { m_FarZ = a_FarZ; m_Dirty = true; }
	const glm::mat4& GetProjection() const;
	glm::mat4 GetView() const;
	glm::mat4 GetProjectionView() const;

private:

	glm::mat4 m_Projection;
	bool      m_Dirty;
	float     m_FOV;
	float     m_Aspect;
	float     m_NearZ;
	float     m_FarZ;

	static Object s_MainCamera;
};