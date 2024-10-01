#include "Renderer3D.h"

#include "Nest/Core/Core.h"
#include "Nest/renderer/Shader.h"
#include "Nest/renderer/VertexArray.h"
#include "Nest/renderer/IndexBuffer.h"
#include "Nest/renderer/RenderCommand.h"

#include "embeddedShaders/BasicVert.h"
#include "embeddedShaders/BasicFrag.h"
#include "embeddedShaders/Axis3D.h"

#include <glad/glad.h>

struct Renderer3DData
{
	Nest::Ref<Nest::Shader> cubeShader, axisShader;
	Nest::Ref<Nest::VertexArray> cubeVertices, axisVertices;
	Nest::Ref<Nest::IndexBuffer> cubeIndices, lineCubeIndices, axisIndices;
};

static Renderer3DData *s_data = nullptr;
static chcl::Mat4 MVP;

void Nest::Renderer3D::init()
{
	if (!s_data)
	{
		s_data = new Renderer3DData();

		Ref<VertexBufferLayout> vec3Layout = createRef<VertexBufferLayout>();
		vec3Layout->push<float>(3);

		chcl::Vector3<float> cubeVertices[8];
		for (unsigned char i = 0; i < 8; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				cubeVertices[i][j] = -0.5f + ((i & (1 << j)) >> j);
			}
		}
		Ref<VertexBuffer> cubeVB = createRef<VertexBuffer>(8 * sizeof(chcl::Vector3<float>), (void*) cubeVertices);

		s_data->cubeVertices = createRef<VertexArray>();
		s_data->cubeVertices->bindBuffers(cubeVB, vec3Layout);

		unsigned int cubeIndexData[3 * 6 * 2] = {
			0, 4, 2,	// -X
			2, 4, 6,	// -X
			1, 3, 5,	// +X
			3, 7, 5,	// +X
			0, 1, 4,	// -Y
			1, 5, 4,	// -Y
			2, 6, 3,	// +Y
			3, 6, 7,	// +Y
			0, 2, 1,	// -Z
			1, 2, 3,	// -Z
			4, 5, 6,	// +Z
			5, 7, 6		// +Z
		};
		s_data->cubeIndices = createRef<IndexBuffer>(3 * 6 * 2, cubeIndexData);

		unsigned int lineCubeIndexData[24] = {
			0, 1,
			0, 2,
			0, 4,
			1, 3,
			1, 5,
			2, 3,
			2, 6,
			3, 7,
			4, 5,
			4, 6,
			5, 7,
			6, 7
		};
		s_data->lineCubeIndices = createRef<IndexBuffer>(24, lineCubeIndexData);

		s_data->cubeShader = createRef<Shader>(std::vector<std::string>({ glslBasicVert_data, glslBasicFrag_data }));

		chcl::Vector3<float> axisVertexData[4] = {
			{ 0.f, 0.f, 0.f },
			{ 1.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ 0.f, 0.f, 1.f }
		};
		Ref<VertexBuffer> axisVB = createRef<VertexBuffer>(sizeof(axisVertexData), (void*) axisVertexData);
			
		s_data->axisVertices = createRef<VertexArray>();
		s_data->axisVertices->bindBuffers(axisVB, vec3Layout);

		unsigned int axisIndexData[6] = {
			0, 1,
			0, 2,
			0, 3
		};
		s_data->axisIndices = createRef<IndexBuffer>(6, axisIndexData);

		s_data->axisShader = createRef<Shader>(glslAxis3D_data);
	}
}

void Nest::Renderer3D::shutdown()
{
	if (s_data)
	{
		delete s_data;
		s_data = nullptr;
	}
}

void Nest::Renderer3D::beginScene(const chcl::Mat4 &mvp)
{
	s_data->cubeShader->bind();
	s_data->cubeShader->setUniformMat4("u_MVP", mvp);

	s_data->axisShader->bind();
	s_data->axisShader->setUniformMat4("u_MVP", mvp);

	MVP = mvp;

	RenderCommand::setDepthEnabled(true);
}

void Nest::Renderer3D::drawAxes(chcl::Vector3<float> pos, float size)
{
	chcl::Mat4 transform = chcl::Mat4::Translation3D(pos) * chcl::Mat4::Scale3D(size);

	s_data->axisShader->bind();
	s_data->axisShader->setUniformMat4("u_transform", transform);

	s_data->axisVertices->bind();
	s_data->axisIndices->bind();
	RenderCommand::drawLinesIndexed(s_data->axisIndices);
}

void Nest::Renderer3D::drawAxes(chcl::Vector3<float> pos, float size, float pitch, float yaw)
{
	chcl::Mat4 transform = chcl::Mat4::Translation3D(pos) * chcl::Mat4::Scale3D(size) * chcl::Mat4::RotationXZ(yaw) * chcl::Mat4::RotationYZ(pitch);

	s_data->axisShader->bind();
	s_data->axisShader->setUniformMat4("u_transform", transform);

	s_data->axisVertices->bind();
	s_data->axisIndices->bind();
	RenderCommand::drawLinesIndexed(s_data->axisIndices);
}

void Nest::Renderer3D::drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, chcl::Vector4<float> color)
{
	chcl::Mat4 transform = chcl::Mat4::Translation3D(pos) * chcl::Mat4::Scale3D(size);

	s_data->cubeShader->bind();
	s_data->cubeShader->setUniform4f("u_color", color);
	s_data->cubeShader->setUniformMat4("u_transform", transform);

	s_data->cubeVertices->bind();
	s_data->cubeIndices->bind();
	RenderCommand::drawTrianglesIndexed(s_data->cubeIndices);
}

void Nest::Renderer3D::drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, float pitch, float yaw, chcl::Vector4<float> color)
{
	chcl::Mat4 transform = chcl::Mat4::Translation3D(pos) * chcl::Mat4::Scale3D(size) * chcl::Mat4::RotationXZ(yaw) * chcl::Mat4::RotationYZ(pitch);

	s_data->cubeShader->bind();
	s_data->cubeShader->setUniform4f("u_color", color);
	s_data->cubeShader->setUniformMat4("u_transform", transform);

	s_data->cubeVertices->bind();
	s_data->cubeIndices->bind();
	RenderCommand::drawTrianglesIndexed(s_data->cubeIndices);
}

void Nest::Renderer3D::drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, chcl::Vector4<float> color, Ref<Shader> shader)
{
	chcl::Mat4 transform = chcl::Mat4::Translation3D(pos) * chcl::Mat4::Scale3D(size);

	shader->bind();
	shader->setUniformMat4("u_MVP", MVP);
	shader->setUniformMat4("u_transform", transform);
	shader->setUniform4f("u_color", color);

	s_data->cubeVertices->bind();
	s_data->cubeIndices->bind();
	RenderCommand::drawTrianglesIndexed(s_data->cubeIndices);
}

void Nest::Renderer3D::drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, float pitch, float yaw, chcl::Vector4<float> color, Ref<Shader> shader)
{
	chcl::Mat4 transform = chcl::Mat4::Translation3D(pos) * chcl::Mat4::Scale3D(size) * chcl::Mat4::RotationXZ(yaw) * chcl::Mat4::RotationYZ(pitch);

	shader->bind();
	shader->setUniformMat4("u_MVP", MVP);
	shader->setUniformMat4("u_transform", transform);
	shader->setUniform4f("u_color", color);

	s_data->cubeVertices->bind();
	s_data->cubeIndices->bind();
	RenderCommand::drawTrianglesIndexed(s_data->cubeIndices);
}

void Nest::Renderer3D::drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, Ref<Shader> shader)
{
	chcl::Mat4 transform = chcl::Mat4::Translation3D(pos) * chcl::Mat4::Scale3D(size);

	shader->bind();
	shader->setUniformMat4("u_MVP", MVP);
	shader->setUniformMat4("u_transform", transform);

	s_data->cubeVertices->bind();
	s_data->cubeIndices->bind();
	RenderCommand::drawTrianglesIndexed(s_data->cubeIndices);
}

void Nest::Renderer3D::drawCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, float pitch, float yaw, Ref<Shader> shader)
{
	chcl::Mat4 transform = chcl::Mat4::Translation3D(pos) * chcl::Mat4::Scale3D(size) * chcl::Mat4::RotationXZ(yaw) * chcl::Mat4::RotationYZ(pitch);

	shader->bind();
	shader->setUniformMat4("u_MVP", MVP);
	shader->setUniformMat4("u_transform", transform);

	s_data->cubeVertices->bind();
	s_data->cubeIndices->bind();
	RenderCommand::drawTrianglesIndexed(s_data->cubeIndices);
}

void Nest::Renderer3D::drawLineCuboid(chcl::Vector3<float> pos, chcl::Vector3<float> size, chcl::Vector4<float> color)
{
	chcl::Mat4 transform = chcl::Mat4::Translation3D(pos) * chcl::Mat4::Scale3D(size);

	s_data->cubeShader->bind();
	s_data->cubeShader->setUniform4f("u_color", color);
	s_data->cubeShader->setUniformMat4("u_transform", transform);

	s_data->cubeVertices->bind();
	s_data->lineCubeIndices->bind();
	RenderCommand::drawLinesIndexed(s_data->lineCubeIndices);
}

void Nest::Renderer3D::drawMesh(chcl::Vector3<float> offset, chcl::Vector3<float> scale, const StaticMesh &mesh, Ref<Shader> shader)
{
	chcl::Mat4 transform = chcl::Mat4::Translation3D(offset) * chcl::Mat4::Scale3D(scale);

	shader->bind();
	shader->setUniformMat4("u_transform", transform);
	shader->setUniformMat4("u_MVP", MVP);

	mesh.getVA()->bind();
	mesh.getIB()->bind();
	RenderCommand::drawTrianglesIndexed(mesh.getIB());
}
