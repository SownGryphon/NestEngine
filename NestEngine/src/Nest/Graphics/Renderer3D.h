#pragma once

#include <maths/Mat4.h>
#include <geometry/Vector3.h>
#include <geometry/Vector4.h>

#include "Shader.h"
#include "Nest/Graphics/StaticMesh.h"

namespace Nest
{
	namespace Renderer3D
	{
		void init();
		void shutdown();

		void beginScene(const chcl::Mat4 &mvp);

		void drawAxes(chcl::Vector3<float> pos, float size);
		void drawAxes(chcl::Vector3<float> pos, float size, float pitch, float yaw);

		void drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, chcl::Vector4<float> color);
		void drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, float pitch, float yaw, chcl::Vector4<float> color);
		void drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, chcl::Vector4<float> color, Ref<Shader> shader);
		void drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, float pitch, float yaw, chcl::Vector4<float> color, Ref<Shader> shader);
		void drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, Ref<Shader> shader);
		void drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, float pitch, float yaw, Ref<Shader> shader);
		void drawLineCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, chcl::Vector4<float> color);

		void drawMesh(chcl::Vector3<float> offset, chcl::Vector3<float> scale, const StaticMesh &mesh, Ref<Shader> shader);
	}
}