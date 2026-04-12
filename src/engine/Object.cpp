#include "Object.h"

namespace Engine {
    Object::Object(const std::string& name)
        : m_name(name)
    {
    }

    void Object::update(float deltaTime)
    {
        
    }

    void Object::setMesh(std::shared_ptr<RenderEngine::Mesh> newMesh)
    {
        mesh = std::move(newMesh);
    }

    void Object::setMaterial(std::shared_ptr<RenderEngine::Material> newMaterial)
    {
        material = std::move(newMaterial);
    }
}