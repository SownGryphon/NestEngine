#include "RenderCommand.h"

#include <glad/glad.h>

namespace Nest
{
	void RenderCommand::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderCommand::drawPoints(unsigned int count)
	{
		glDrawArrays(GL_POINTS, 0, count);
	}
	
	void RenderCommand::drawLines(unsigned int count)
	{
		glDrawArrays(GL_LINES, 0, 2 * count);
	}

	void RenderCommand::drawLinesIndexed(Ref<IndexBuffer>& ib)
	{
		glDrawElements(GL_LINES, ib->getCount(), GL_UNSIGNED_INT, 0);
	}

	void RenderCommand::drawTriangles(unsigned int count)
	{
		glDrawArrays(GL_TRIANGLES, 0, 3 * count);
	}

	void RenderCommand::drawTrianglesIndexed(Ref<IndexBuffer>& ib)
	{
		glDrawElements(GL_TRIANGLES, ib->getCount(), GL_UNSIGNED_INT, 0);
	}

}
