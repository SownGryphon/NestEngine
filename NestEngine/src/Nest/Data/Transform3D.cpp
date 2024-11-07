#include "Nest/Data/Transform3D.h"

#include "Nest/Core/Resource/StaticResourceProvider.h"

Nest::Transform3D::Transform3D() :
	Transform3D(chcl::Vector3<float>(), chcl::Vector3<float>(1.f), chcl::Quaternion(0.f, 1.f, 1.f, 1.f))
{}

Nest::Transform3D::Transform3D(chcl::Vector3<float> position, chcl::Vector3<float> scale, chcl::Quaternion rotation) :
	m_translation(ResourceProvider<chcl::Vector3<float>>(position)),
	m_scale(ResourceProvider<chcl::Vector3<float>>(scale)),
	m_rotation(ResourceProvider<chcl::Quaternion>(rotation))
{}

Nest::Transform3D::Transform3D(TranslationType translationSource, ScaleType scaleSource, RotationType rotationSource) :
	m_translation(translationSource),
	m_scale(scaleSource),
	m_rotation(rotationSource)
{}

chcl::Mat4 Nest::Transform3D::getTransform() const
{
	updateTransformMat();
	return m_transformMat;
}

void Nest::Transform3D::updateTransformMat() const
{
	if (
		!m_translation.isUpToDate()
		|| !m_rotation.isUpToDate()
		|| !m_scale.isUpToDate()
		)
	{
		m_transformMat = chcl::Mat4::Translation3D(m_translation.get())
			* chcl::Mat4::Rotation3D(m_rotation.get())
			* chcl::Mat4::Scale3D(m_scale.get());
	}
}