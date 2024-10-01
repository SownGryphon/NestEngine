#pragma once

#include "geometry/Vector3.h"
#include "maths/Mat4.h"

namespace Nest
{
	class PerspectiveCamera3D
	{
	public:
		PerspectiveCamera3D() = default;
		PerspectiveCamera3D(float FOV, float aspectRatio, float near, float far, chcl::Vector3<float> pos, float pitch, float yaw);
		PerspectiveCamera3D(const PerspectiveCamera3D &cam) = default;

		inline void setFOV(float fov) { m_FOV = fov; recalculateMatrix(); }
		inline void setAspectRatio(float aspect) { m_aspect = aspect; recalculateMatrix(); }
		inline void setNearDist(float near) { m_near = near; recalculateMatrix(); }
		inline void setFarDist(float far) { m_far = far; recalculateMatrix(); }
		inline void setPos(chcl::Vector3<float> pos) { m_pos = pos; recalculateMatrix(); }
		inline void setPitch(float pitch) { m_pitch = pitch; recalculateMatrix(); }
		inline void setYaw(float yaw) { m_yaw = yaw; recalculateMatrix(); }
		inline void setDir(float pitch, float yaw) { m_pitch = pitch; m_yaw = yaw; recalculateMatrix(); }

		inline float getFOV() const { return m_FOV; }
		inline float getAspectRatio() const { return m_aspect; }
		inline float getNearDist() const { return m_near; }
		inline float getFarDist() const { return m_far; }
		inline chcl::Vector3<float> getPos() const { return m_pos; }
		inline float getPitch() const { return m_pitch; }
		inline float getYaw() const { return m_yaw; }

		chcl::Mat4 getRotationMat() const;
		chcl::Mat4 getRotationMatInv() const;
		inline const chcl::Mat4& getVPM() const { return m_VPM; }

		PerspectiveCamera3D& operator=(const PerspectiveCamera3D &cam) = default;

	protected:
		float m_FOV, m_aspect, m_near, m_far;
		chcl::Vector3<float> m_pos;
		float m_pitch, m_yaw;

		// View projection matrix
		chcl::Mat4 m_VPM;

		void recalculateMatrix();
	};
}