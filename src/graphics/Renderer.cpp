#include "Renderer.h"
#include "Mesh.h"
namespace RenderEngine {
	void Renderer::draw(Mesh& mesh, Material& material) {
		mesh.bind();
		if (mesh.isIndexed()) {
			glDrawElements(GL_TRIANGLES, mesh.getIndexCount(), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());
		}
		mesh.unbind();
	}

	void Renderer::setDepthTest(const bool enable) {
		if (enable) {
			glEnable(GL_DEPTH_TEST);
		}else glDisable(GL_DEPTH_TEST);
	}

	void Renderer::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::setViewport(const unsigned int width, const unsigned int height)
	{
		glViewport(0, 0, width, height);
	}




}