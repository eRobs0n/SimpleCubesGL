#include "Material.h"
#include "glutils.h"

namespace RenderEngine
{
    Material::Material(std::shared_ptr<GLSLProgram> shader, const std::string& name) : name(name), shader(shader) {}
    void Material::setColor(const glm::vec3 & color) {
        albedoColor = color;
    }
    void Material::use() {
        shader->use();
        shader->setUniform("albedoColor", albedoColor);

    }
    void Material::setMatrices(const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& projection) const
    {
        shader->setUniform("model", model);
        shader->setUniform("view", view);
        shader->setUniform("projection", projection);
    }
}