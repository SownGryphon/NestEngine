#pragma once

#include <geometry/Vector3.h>
#include <geometry/Quaternion.h>
#include <maths/Mat4.h>

#include "Nest/Core/Core.h"
#include "Nest/Core/Resource/ResourceProvider.h"

namespace Nest
{
	class Transform3D
	{
	private:
		using TranslationType = ResourceProvider<chcl::Vector3<float>>;
		using ScaleType = ResourceProvider<chcl::Vector3<float>>;
		using RotationType = ResourceProvider<chcl::Quaternion>;

		TranslationType m_translation;
		ScaleType m_scale;
		ResourceProvider<chcl::Quaternion> m_rotation;
		mutable chcl::Mat4 m_transformMat;

	public:
		Transform3D();
		Transform3D(chcl::Vector3<float> translation, chcl::Vector3<float> scale, chcl::Quaternion rotation);
		Transform3D(TranslationType translationSource, ScaleType scaleSource, RotationType rotationSource);

		inline chcl::Vector3<float> getTranslation() const { return m_translation.peek(); }
		inline chcl::Vector3<float> getScale() const { return m_scale.peek(); }
		inline chcl::Quaternion getRotation() const { return m_rotation.peek(); }

		chcl::Mat4 getTransform() const;

		inline void setTranslation(chcl::Vector3<float> translation) { m_translation.set(translation); }
		inline void setScale(chcl::Vector3<float> scale) { m_scale.set(scale); }
		inline void setRotation(chcl::Quaternion rotation) { m_rotation.set(rotation); }

		inline void setTranslationSource(TranslationType translationSource) { m_translation = translationSource; }
		inline void setScaleSource(ScaleType scaleSource) { m_scale = scaleSource; }
		inline void setRotationSource(RotationType rotationSource) { m_rotation = rotationSource; }

	private:
		void updateTransformMat() const;
	};
}