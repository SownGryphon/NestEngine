#include "Renderer2D.h"

#include <Prints.h>
#include <maths/SquareMatrix.h>

#include "Nest/Core/Core.h"
#include "Nest/Core/Application.h"

#include "Nest/renderer/Shader.h"
#include "Nest/renderer/VertexArray.h"
#include "Nest/renderer/IndexBuffer.h"
#include "Nest/renderer/RenderCommand.h"

#include "embeddedShaders/BasicVert.h"
#include "embeddedShaders/BasicFrag.h"
#include "embeddedShaders/Circle.h"
#include "embeddedShaders/TexturedQuad.h"
#include "embeddedShaders/Font.h"

namespace Nest
{
	struct Renderer2DData
	{
		Ref<Shader> quadShader, circleShader, texturedQuadShader, textShader;
		Ref<VertexArray> squareVertices, lineVertices;
		Ref<IndexBuffer> quadIndices, lineQuadIndices;
		chcl::Vector2<unsigned int> windowSize;
	};

	static Renderer2DData *s_data = nullptr;
	static chcl::Mat4 MVP;

	void Renderer2D::init()
	{
		if (!s_data)
		{
			s_data = new Renderer2DData();

			s_data->lineVertices = createRef<VertexArray>();
			Ref<VertexBuffer> lineVB = createRef<VertexBuffer>(4 * sizeof(float));
			Ref<VertexBufferLayout> lineVBL = createRef<VertexBufferLayout>();
			lineVBL->push<float>(2);
			s_data->lineVertices->bindBuffers(lineVB, lineVBL);

			s_data->squareVertices = createRef<VertexArray>();
			float positions[] = {
				-0.5f, -0.5f, 0.f, 0.f,
				-0.5f,  0.5f, 0.f, 1.f,
				0.5f,  0.5f, 1.f, 1.f,
				0.5f, -0.5f, 1.f, 0.f
			};
			Ref<VertexBuffer> squareVB = createRef<VertexBuffer>(sizeof(positions), positions);
			Ref<VertexBufferLayout> quadVBL = createRef<VertexBufferLayout>();
			quadVBL->push<float>(2);
			quadVBL->push<float>(2);
			s_data->squareVertices->bindBuffers(squareVB, quadVBL);

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

			s_data->windowSize.x = Application::GetInstance().getWindow().getWidth();
			s_data->windowSize.y = Application::GetInstance().getWindow().getHeight();

			s_data->quadShader = createRef<Shader>(std::vector<Shader::ShaderSource>({
				{ glslBasicVert_data, "Basic Vertex" },
				{ glslBasicFrag_data, "Basic Fragment" }
			}));

			s_data->circleShader = createRef<Shader>(Shader::ShaderSource{ glslCircle_data, "Circle" });

			s_data->texturedQuadShader = createRef<Shader>(Shader::ShaderSource{ glslTexQuad_data, "Textured Quad" });

			s_data->textShader = createRef<Shader>(Shader::ShaderSource{ glslFont_data, "Font" });
		}
	}

	void Renderer2D::shutdown()
	{
		if (s_data)
			delete s_data;
	}

	void Renderer2D::beginScene(const chcl::Mat4 &mvp)
	{
		if (mvp != MVP)
		{
			s_data->quadShader->bind();
			s_data->quadShader->setUniformMat4("u_MVP", mvp);

			s_data->circleShader->bind();
			s_data->circleShader->setUniformMat4("u_MVP", mvp);

			s_data->texturedQuadShader->bind();
			s_data->texturedQuadShader->setUniformMat4("u_MVP", mvp);

			s_data->textShader->bind();
			s_data->textShader->setUniformMat4("u_MVP", mvp);

			MVP = mvp;
		}

		//RenderCommand::setDepthEnabled(false);
	}

	void Renderer2D::endScene()
	{
	}

	void Renderer2D::drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation3D(pos.x, pos.y, 0) * chcl::Mat4::Scale3D(size.x, size.y, 1);
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_transform", transform);
		s_data->quadShader->setUniform4f("u_color", color);

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Renderer2D::drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, Ref<Shader> shader)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation3D(pos.x, pos.y, 0) * chcl::Mat4::Scale3D(size.x, size.y, 1);
		shader->bind();
		shader->setUniformMat4("u_MVP", MVP);
		shader->setUniformMat4("u_transform", transform);

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Renderer2D::drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, chcl::Vector4<float> color)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation3D(pos.x, pos.y, 0);
		transform *= chcl::Mat4::RotationXY(rotation);
		transform *= chcl::Mat4::Scale3D(size.x, size.y, 1);
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_transform", transform);
		s_data->quadShader->setUniform4f("u_color", color);

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Renderer2D::drawQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, Ref<Shader> shader)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation3D(pos.x, pos.y, 0);
		transform *= chcl::Mat4::RotationXY(rotation);
		transform *= chcl::Mat4::Scale3D(size.x, size.y, 1);
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_MVP", MVP);
		s_data->quadShader->setUniformMat4("u_transform", transform);

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Renderer2D::drawLineQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, chcl::Vector4<float> color, float weight)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation3D(pos.x, pos.y, 0) * chcl::Mat4::Scale3D(size.x, size.y, 1);
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_transform", transform);
		s_data->quadShader->setUniform4f("u_color", color);

		s_data->squareVertices->bind();
		s_data->lineQuadIndices->bind();
		RenderCommand::setLineWidth(weight);
		RenderCommand::drawLinesIndexed(s_data->lineQuadIndices);
	}

	void Renderer2D::drawLineQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, float rotation, chcl::Vector4<float> color, float weight)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation3D(pos.x, pos.y, 0);
		transform *= chcl::Mat4::RotationXY(rotation);
		transform *= chcl::Mat4::Scale3D(size.x, size.y, 1);
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_transform", transform);
		s_data->quadShader->setUniform4f("u_color", color);

		s_data->squareVertices->bind();
		s_data->lineQuadIndices->bind();
		RenderCommand::setLineWidth(weight);
		RenderCommand::drawLinesIndexed(s_data->lineQuadIndices);
	}

	void Renderer2D::drawCircle(chcl::Vector2<float> pos, chcl::Vector2<float> size, float thickness, chcl::Vector4<float> fillColor, chcl::Vector4<float> outlineColor)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation3D(pos.x, pos.y, 0) * chcl::Mat4::Scale3D(size.x, size.y, 1);
		s_data->circleShader->bind();
		s_data->circleShader->setUniformMat4("u_transform", transform);
		s_data->circleShader->setUniform4f("u_fillColor", fillColor);
		s_data->circleShader->setUniform4f("u_outlineColor", outlineColor);
		s_data->circleShader->setUniform2f("u_outlineWidth", thickness / size * 2);

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Nest::Renderer2D::drawTexturedQuad(chcl::Vector2<float> pos, chcl::Vector2<float> size, Ref<Texture> texture)
	{
		chcl::Mat4 transform = chcl::Mat4::Translation3D(pos.x, pos.y, 0) * chcl::Mat4::Scale3D(size.x, size.y, 1);
		s_data->texturedQuadShader->bind();
		s_data->texturedQuadShader->setUniformMat4("u_transform", transform);

		texture->bind(0);

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Renderer2D::drawFullScreenQuad()
	{
		s_data->quadShader->bind();
		chcl::Mat4 transform = chcl::Mat4::Translation3D(float(s_data->windowSize.x) / 2, float(s_data->windowSize.y) / 2, 0.);
		transform *= chcl::Mat4::Scale3D(float(s_data->windowSize.x), float(s_data->windowSize.y), 1.f);
		s_data->quadShader->setUniformMat4("u_transform", transform);

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Renderer2D::drawFullScreenQuad(Ref<Shader> shader)
	{
		shader->bind();
		shader->setUniformMat4("u_MVP", chcl::Mat4::Scale3D(2.f, 2.f, 1.f));
		shader->setUniformMat4("u_transform", chcl::Mat4::Identity());

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Renderer2D::drawFullScreenQuad(chcl::Vector4<float> color)
	{
		s_data->quadShader->bind();
		chcl::Mat4 transform = chcl::Mat4::Translation3D(float(s_data->windowSize.x) / 2, float(s_data->windowSize.y) / 2, 0.) * chcl::Mat4::Scale3D(float(s_data->windowSize.x), float(s_data->windowSize.y), 1.f);
		s_data->quadShader->setUniformMat4("u_transform", transform);
		s_data->quadShader->setUniform4f("u_color", color);

		s_data->squareVertices->bind();
		s_data->quadIndices->bind();
		RenderCommand::drawTrianglesIndexed(s_data->quadIndices);
	}

	void Renderer2D::drawLine(chcl::Vector2<float> v1, chcl::Vector2<float> v2, chcl::Vector4<float> color, float width)
	{
		s_data->quadShader->bind();
		s_data->quadShader->setUniformMat4("u_transform", chcl::Mat4::Identity());
		s_data->quadShader->setUniform4f("u_color", color);

		auto vb = s_data->lineVertices->getVertexBuffer();
		s_data->lineVertices->bind();
		vb->setData(2 * sizeof(float), &v1, 0);
		vb->setData(2 * sizeof(float), &v2, sizeof(v1));

		RenderCommand::setLineWidth(width);
		RenderCommand::drawLines(1);
	}
}

void Nest::Renderer2D::drawText(chcl::Vector2<float> pos, float pnt, const std::string &text, Ref<Font> font)
{
	Ref<VertexArray> textVA = font->generateVertexArray(text);
	Ref<IndexBuffer> textIB = font->generateIndexBuffer(text);

	float fUnitScale = pnt / font->getUnitsPerEm();
	chcl::Mat4 transform = chcl::Mat4::Translation3D(pos.x, pos.y, 0.f) * chcl::Mat4::Scale3D(fUnitScale, fUnitScale, 0.f);

	s_data->textShader->bind();
	s_data->textShader->setUniformMat4("u_transform", transform);
	s_data->textShader->setUniform1f("u_fuPerPx", 1.f / fUnitScale);

	font->getFontBuffer()->bind();
	s_data->textShader->bindBuffer(font->getFontBuffer(), 1);

	textVA->bind();
	textIB->bind();

	RenderCommand::drawTrianglesIndexed(textIB);
}

void Nest::Renderer2D::drawTextBox(const TextBox &box)
{
	s_data->textShader->bind();
	s_data->textShader->setUniformMat4("u_transform", box.getTransform());
	s_data->textShader->setUniform1f("u_fuPerPx", box.getFont()->getUnitsPerEm() / box.getPnt());

	box.getFont()->getFontBuffer()->bind();
	s_data->textShader->bindBuffer(box.getFont()->getFontBuffer(), 1);

	box.getVA()->bind();
	box.getIB()->bind();

	RenderCommand::drawTrianglesIndexed(box.getIB());
}