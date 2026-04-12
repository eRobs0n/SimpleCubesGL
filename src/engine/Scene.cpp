#include "Scene.h"

namespace Engine
{
    Scene::Scene(unsigned int width, unsigned int height)
        : m_viewportWidth(width), m_viewportHeight(height)
    {
    }

    void Scene::setViewport(unsigned int width, unsigned int height)
    {
        m_viewportWidth = width;
        m_viewportHeight = height;
        m_renderer.setViewport(width, height);
    }

    void Scene::addObject(const std::shared_ptr<Object>& object)
    {
        if (!object)
            return;

        m_objects.push_back(object);
    }

    void Scene::removeObject(const std::shared_ptr<Object>& object)
    {
        m_objects.erase(
            std::remove(m_objects.begin(), m_objects.end(), object),
            m_objects.end()
        );
    }

    void Scene::clear()
    {
        m_objects.clear();
    }

    void Scene::update(float deltaTime)
    {
        for (auto& object : m_objects)
        {
            if (object)
                object->update(deltaTime);
        }
    }

    void Scene::render()
    {
        m_renderer.clear();

        float aspect = (float)m_viewportWidth / (float)m_viewportHeight;

        glm::mat4 view = m_camera.getViewMatrix();
        glm::mat4 projection = m_camera.getProjectionMatrix(aspect);

        for (const auto& object : m_objects)
        {
            if (!object || !object->isVisible())
                continue;

            if (!object->mesh || !object->material)
                continue;

            glm::mat4 model = object->transform.getModelMatrix();

            object->material->setMatrices(model, view, projection);

            m_renderer.draw(*object->mesh, *object->material);
        }
    }
}