#pragma once
#include "shader.h"
#include <string>
#include <Material.h>
namespace RenderEngine {
	class Renderer {
	public:
        Renderer() = default;
        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;
        ~Renderer() = default;

        void draw(const Mesh& mesh, const Material& material);
        void setDepthTest(const bool enable);
        void clear();
        void setViewport(unsigned int width, unsigned int height);

	};
}