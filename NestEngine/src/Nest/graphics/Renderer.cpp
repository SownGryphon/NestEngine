#include "Renderer.h"

#include <glad/glad.h>

#include "Nest/Graphics/Renderer2D.h"

namespace Nest
{
	static chcl::Mat4 viewProjectionMatrix = chcl::Mat4::Identity();

	void Renderer::init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Renderer2D::init();
	}

	void Renderer::shutdown()
	{
		Renderer2D::shutdown();
	}

	void Renderer::beginScene(const OrthograhicCamera &camera)
	{
		viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::drawIndexed(Shader &shader, const VertexArray &va, const IndexBuffer &ib)
	{
		shader.bind();
		shader.setUniformMat4("u_MVP", viewProjectionMatrix);
		va.bind();
		ib.bind();
		glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
	}

	void Renderer::drawLinesIndexed(Shader &shader, const VertexArray &va, const IndexBuffer &ib)
	{
		shader.bind();
		shader.setUniformMat4("u_MVP", viewProjectionMatrix);
		va.bind();
		ib.bind();
		glDrawElements(GL_LINES, ib.getCount(), GL_UNSIGNED_INT, 0);
	}

	void Renderer::drawPoints(Shader &shader, const VertexArray &va, unsigned int count)
	{
		shader.bind();
		shader.setUniformMat4("u_MVP", viewProjectionMatrix);
		va.bind();
		glDrawArrays(GL_POINTS, 0, count);
	}

	void Renderer::drawLines(Shader &shader, const VertexArray &va, unsigned int vertices)
	{
		shader.bind();
		shader.setUniformMat4("u_MVP", viewProjectionMatrix);
		va.bind();
		glDrawArrays(GL_LINES, 0, vertices);
	}

	void Renderer::drawTriangles(Shader &shader, const VertexArray &va, unsigned int count)
	{
		shader.bind();
		shader.setUniformMat4("u_MVP", viewProjectionMatrix);
		va.bind();
		glDrawArrays(GL_TRIANGLES, 0, 3 * count);
	}
}