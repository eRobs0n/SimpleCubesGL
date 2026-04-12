#pragma once
#include "shader.h"
namespace RenderEngine
{
    class Material
    {
    public:
        std::string name = "UnnamedMaterial";

        std::shared_ptr<GLSLProgram> shader;

      //  std::shared_ptr<Texture> diffuseMap;   // albedo / base color

        glm::vec3 albedoColor = glm::vec3(1.0f);

    public:
        Material() = default;
        Material(std::shared_ptr<GLSLProgram> shader, const std::string& name = "UnnamedMaterial");
        void use();     
      //  void setTexture(const std::string& name, std::shared_ptr<Texture> texture);
        void setColor(const glm::vec3& color);
        void setMatrices(const glm::mat4& model,
            const glm::mat4& view,
            const glm::mat4& projection) const;
    };

}