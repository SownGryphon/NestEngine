#include "Renderer.h"

#include <glad/glad.h>

#include "Nest/renderer/Renderer2D.h"
#include "Nest/renderer/Renderer3D.h"

namespace Nest
{
	static chcl::Mat4 viewProjectionMatrix = chcl::Mat4::Identity();

	unsigned int g_windowWidth = 0, g_windowHeight = 0;

	void Renderer::init(unsigned int windowWidth, unsigned int windowHeight)
	{
		g_windowWidth = windowWidth;
		g_windowHeight = windowHeight;

		glEnable(GL_BLEND);
		(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Renderer2D::init();
		Renderer3D::init();
	}

	void Renderer::shutdown()
	{
		Renderer2D::shutdown();
		Renderer3D::shutdown();
	}

	unsigned int Renderer::getWindowWidth()
	{
		return g_windowWidth;
	}

	unsigned int Renderer::getWindowHeight()
	{
		return g_windowHeight;
	}

	chcl::Vector2<unsigned int> Renderer::getWindowSize()
	{
		return { g_windowWidth, g_windowHeight };
	}

	bool Renderer::onWindowResize(WindowResizeEvent &e)
	{
		g_windowWidth = e.getWidth();
		g_windowHeight = e.getHeight();

		return false;
	}

	void Renderer::beginScene(const chcl::Mat4 &vpm)
	{
		viewProjectionMatrix = vpm;
	}

	void Renderer::drawIndexed(Ref<Shader> shader, Ref<const VertexArray> va, const IndexBuffer &ib)
	{
		shader->bind();
		shader->setUniformMat4("u_MVP", viewProjectionMatrix);
		va->bind();
		ib.bind();
		glDrawElements(GL_TRIANGLES, (GLuint)ib.getCount(), GL_UNSIGNED_INT, 0);
	}

	void Renderer::drawLinesIndexed(Ref<Shader> shader, Ref<const VertexArray> va, const IndexBuffer &ib)
	{
		shader->bind();
		shader->setUniformMat4("u_MVP", viewProjectionMatrix);
		va->bind();
		ib.bind();
		glDrawElements(GL_LINES, (GLuint)ib.getCount(), GL_UNSIGNED_INT, 0);
	}

	void Renderer::drawPoints(Ref<Shader> shader, Ref<const VertexArray> va, unsigned int count)
	{
		shader->bind();
		shader->setUniformMat4("u_MVP", viewProjectionMatrix);
		va->bind();
		glDrawArrays(GL_POINTS, 0, count);
	}

	void Renderer::drawLines(Ref<Shader> shader, Ref<const VertexArray> va, unsigned int vertices)
	{
		shader->bind();
		shader->setUniformMat4("u_MVP", viewProjectionMatrix);
		va->bind();
		glDrawArrays(GL_LINES, 0, vertices);
	}

	void Renderer::drawTriangles(Ref<Shader> shader, Ref<const VertexArray> va, unsigned int count)
	{
		shader->bind();
		shader->setUniformMat4("u_MVP", viewProjectionMatrix);
		va->bind();
		glDrawArrays(GL_TRIANGLES, 0, 3 * count);
	}
}