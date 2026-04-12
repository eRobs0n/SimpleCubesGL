#pragma once
#include "glm/glm.hpp"
namespace RenderEngine
{

    class Camera
    {
    public:
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        float yaw = -90.0f;   // поворот по горизонтали
        float pitch = 0.0f;     // поворот по вертикали

        float fov = 60.0f;   // Field of View
        float nearPlane = 0.1f;
        float farPlane = 1000.0f;

        mutable glm::mat4 viewMatrix;
        mutable glm::mat4 projectionMatrix;
        mutable bool matricesDirty = true;

        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    public:
        Camera();
        void moveForward(float delta);
        void moveRight(float delta);
        void moveUp(float delta);
        void rotate(float yaw, float pitch);
        glm::mat4 getViewMatrix();
        glm::mat4 getProjectionMatrix(float aspectRatio);
        void updateCameraVectors();
        void setPosition(const glm::vec3& pos);
        void lookAt(const glm::vec3& target);
    };

}