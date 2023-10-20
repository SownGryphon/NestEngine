#include "Renderer2D.h"

#include <Prints.h>
#include <maths/SquareMatrix.h>

#include "Nest/Core/Core.h"

#include "Nest/Graphics/Shader.h"
#include "Nest/Graphics/VertexArray.h"
#include "Nest/Graphics/IndexBuffer.h"
#include "Nest/Graphics/RenderCommand.h"

namespace Nest
{
	struct RendererData
	{
		Ref<Shader> quadShader;
		Ref<VertexArray> squareVertices;
		Ref<IndexBuffer> quadIndices, lineQuadIndices;
	};

	RendererData *s_data;

	void Renderer2D::init()
	{
		s_data = new RendererData();
		s_data->quadShader = createRef<Shader>("res/shaders/Basic.glsl.vertex", "res/shaders/Basic.glsl.fragment");

		s_data->squareVertices = createRef<VertexArray>();
		float positions[8] = {
			-0.5f, -0.5f,
			-0.5f,  0.5f,
				0.5f,  0.5f,
				0.5f, -0.5f
		};
		Ref<VertexBuffer> squareVB = createRef<VertexBuffer>(8 * sizeof(float), positions);
		Ref<VertexBufferLayout> vbl = createRef<VertexBufferLayout>();
		vbl->push<float>(2, 0);
		s_data->squareVertices->bindBuffers(squareVB, vbl);

		unsigned int quadIndices[6] = {
			0, 1, 2,
			2, 3, 0
		};
		s_data->quadIndices = createRef<IndexBuffer>(6, quadIndices);

		unsigned int lineQuadIndices[8] = {
			0, 1,
			1, 2,
			2, 3,
			3, 0
		};
		s_data->lineQuadIndices = createRef<IndexBuffer>(8, lineQuadIndices);
	}

	void Renderer2D::shutdown()
	{
		delete s_data;
	}

	void Renderer2D::beginScene(const OrthograhicCamera & camera)
	{
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_MVP", camera.getViewProjectionMatrix());
	}

	void Renderer2D::endScene()
	{
	}

	void Renderer2D::drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation(pos.x, pos.y, 0) * chcl::Mat4::Scale(size.x, size.y, 1);
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_transform", transform);
		s_data->quadShader->setUniform4f("u_color", color);

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Renderer2D::drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, chcl::Vector4<float> color)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation(pos.x, pos.y, 0);
		transform *= chcl::Mat4::Rotation2D(rotation);
		transform *= chcl::Mat4::Scale(size.x, size.y, 1);
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_transform", transform);
		s_data->quadShader->setUniform4f("u_color", color);

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Renderer2D::drawLineQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation(pos.x, pos.y, 0) * chcl::Mat4::Scale(size.x, size.y, 1);
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_transform", transform);
		s_data->quadShader->setUniform4f("u_color", color);

		s_data->squareVertices->bind();
		s_data->lineQuadIndices->bind();
		RenderCommand::drawLinesIndexed(s_data->lineQuadIndices);
	}

	void Renderer2D::drawLineQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, chcl::Vector4<float> color)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation(pos.x, pos.y, 0);
		transform *= chcl::Mat4::Rotation2D(rotation);
		transform *= chcl::Mat4::Scale(size.x, size.y, 1);
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_transform", transform);
		s_data->quadShader->setUniform4f("u_color", color);

		s_data->squareVertices->bind();
		s_data->lineQuadIndices->bind();
		RenderCommand::drawLinesIndexed(s_data->lineQuadIndices);
	}
}