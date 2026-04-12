#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>

namespace RenderEngine
{
    Camera::Camera()
    {
        updateCameraVectors();
    }

    void Camera::updateCameraVectors()
    {
        // Стандартный расчёт направления: yaw = -90° смотрит вдоль -Z
        glm::vec3 direction;
        direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        direction.y = sin(glm::radians(pitch));
        direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

        front = glm::normalize(direction);

        // Пересчёт локальных осей
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));

        matricesDirty = true;
    }

    glm::mat4 Camera::getViewMatrix()
    {
        if (matricesDirty)
        {
            viewMatrix = glm::lookAt(position, position + front, up);
            matricesDirty = false;
        }
        return viewMatrix;
    }

    glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
    {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void Camera::setPosition(const glm::vec3& pos)
    {
        position = pos;
        matricesDirty = true;
    }

    void Camera::lookAt(const glm::vec3& target)
    {
        front = glm::normalize(target - position);
        // Извлекаем углы из вектора направления (для синхронизации)
        pitch = glm::degrees(asinf(front.y));
        yaw = glm::degrees(atan2(front.z, front.x));
        updateCameraVectors(); // пересчитает right/up и выставит флаг
    }

    void Camera::moveForward(float delta)
    {
        position += front * delta;
        matricesDirty = true;
    }

    void Camera::moveRight(float delta)
    {
        position += right * delta;
        matricesDirty = true;
    }

    void Camera::moveUp(float delta)
    {
        position += up * delta;
        matricesDirty = true;
    }

    void Camera::rotate(float yawOffset, float pitchOffset)
    {
        yaw += yawOffset;
        pitch += pitchOffset;

        // Ограничение угла тангажа, чтобы избежать переворота
        const float maxPitch = 89.0f;
        pitch = std::clamp(pitch, -maxPitch, maxPitch);

        updateCameraVectors();
    }
}