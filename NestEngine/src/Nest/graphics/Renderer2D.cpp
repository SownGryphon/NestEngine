#include "Renderer2D.h"

#include <Prints.h>
#include <maths/SquareMatrix.h>

#include "../Core.h"

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Renderer.h"

namespace Nest
{
	namespace Renderer2D
	{
		struct RendererData
		{
			Ref<Shader> quadShader;
			Ref<VertexArray> squareVertices;
			Ref<IndexBuffer> quadIndices;
		};

		RendererData *s_data;

		void init()
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

			unsigned int indices[6] = {
				0, 1, 2,
				2, 3, 0
			};
			s_data->quadIndices = createRef<IndexBuffer>(6, indices);
		}

		void shutdown()
		{
			delete s_data;
		}

		void drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color)
		{
			chcl::Mat4 transform = chcl::Mat4::Translation(pos.x, pos.y, 0) * chcl::Mat4::Scale(size.x, size.y, 1);
			s_data->quadShader->bind();
			s_data->quadShader->setUniformMat4("u_transform", transform);
			s_data->quadShader->setUniform4f("u_color", color);
			Renderer::drawIndexed(*s_data->quadShader, *s_data->squareVertices, *s_data->quadIndices);
		}

		void drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, chcl::Vector4<float> color)
		{
			chcl::Mat4 transform = chcl::Mat4::Translation(pos.x, pos.y, 0);
			transform *= chcl::Mat4::Rotation2D(rotation);
			transform *= chcl::Mat4::Scale(size.x, size.y, 1);
			s_data->quadShader->bind();
			s_data->quadShader->setUniformMat4("u_transform", transform);
			s_data->quadShader->setUniform4f("u_color", color);
			Renderer::drawIndexed(*s_data->quadShader, *s_data->squareVertices, *s_data->quadIndices);
		}
	}
}