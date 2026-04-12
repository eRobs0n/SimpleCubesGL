#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
namespace Engine {
    struct Transform
    {
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        glm::mat4 getModelMatrix() const
        {
            glm::mat4 mat(1.0f);

            mat = glm::translate(mat, position);
            mat *= glm::mat4_cast(rotation);
            mat = glm::scale(mat, scale);

            return mat;
        }
    };
}