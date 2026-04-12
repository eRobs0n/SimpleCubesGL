#pragma once
#include "Object.h"
#include "../graphics/Renderer.h"
#include "Camera.h"
#include <vector>
namespace Engine {
    class Scene
    {
    private:
        std::vector<std::shared_ptr<Object>> m_objects;

        RenderEngine::Camera   m_camera;
        RenderEngine::Renderer m_renderer;

        unsigned int m_viewportWidth = 1280;
        unsigned int m_viewportHeight = 720;

    public:
        Scene() = default;
        explicit Scene(unsigned int width, unsigned int height);

        RenderEngine::Camera& getCamera() { return m_camera; }
        const RenderEngine::Camera& getCamera() const { return m_camera; }

        void setViewport(unsigned int width, unsigned int height);

        void addObject(const std::shared_ptr<Object>& object);
        void removeObject(const std::shared_ptr<Object>& object);
        void clear();

        const std::vector<std::shared_ptr<Object>>& getObjects() const { return m_objects; }

        void update(float deltaTime);
        void render();
    };
}