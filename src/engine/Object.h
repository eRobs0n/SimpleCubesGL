#pragma once
#include "Transform.h"
#include "../graphics/Mesh.h"
#include "../graphics/Material.h"
namespace Engine {

    class Object
    {
    private:
        std::string m_name;
        bool        m_visible = true;

    public:
        Transform transform;

        std::shared_ptr<RenderEngine::Mesh>     mesh;
        std::shared_ptr<RenderEngine::Material> material;


        Object(const std::string& name = "Object");
        virtual ~Object() = default;

        virtual void update(float deltaTime);

        const std::string& getName() const { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        bool isVisible() const { return m_visible; }
        void setVisible(bool visible) { m_visible = visible; }

        void setMesh(std::shared_ptr<RenderEngine::Mesh> newMesh);
        void setMaterial(std::shared_ptr<RenderEngine::Material> newMaterial);
    };
}