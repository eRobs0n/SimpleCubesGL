// main.cpp — FPS-камера: мышь + клавиатура
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "engine/Scene.h"
#include "engine/Object.h"
#include "graphics/Mesh.h"
#include "graphics/Material.h"
#include "graphics/shader.h"

// -------------------------------------------------------
// Константы
// -------------------------------------------------------
static constexpr unsigned int SCR_WIDTH = 1280;
static constexpr unsigned int SCR_HEIGHT = 720;
static constexpr float        CAM_SPEED = 5.0f;   // единиц/сек
static constexpr float        SENSITIVITY = 0.1f;   // градусов/пиксель

// -------------------------------------------------------
// Глобальное состояние ввода мыши
// Минимально — только то, что нужно коллбэку
// -------------------------------------------------------
struct MouseState {
    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool  firstMouse = true;
    float yawDelta = 0.0f;
    float pitchDelta = 0.0f;
};

static MouseState g_mouse;
// Указатель на сцену нужен коллбэкам GLFW
static Engine::Scene* g_scene = nullptr;

// -------------------------------------------------------
// Коллбэки GLFW
// -------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
    if (g_scene)
        g_scene->setViewport(static_cast<unsigned int>(width),
            static_cast<unsigned int>(height));
}

void mouseCallback(GLFWwindow* /*window*/, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (g_mouse.firstMouse) {
        g_mouse.lastX = xpos;
        g_mouse.lastY = ypos;
        g_mouse.firstMouse = false;
    }

    // Накапливаем дельту — применяем в основном цикле
    g_mouse.yawDelta += (xpos - g_mouse.lastX) * SENSITIVITY;
    g_mouse.pitchDelta -= (ypos - g_mouse.lastY) * SENSITIVITY; // ось Y перевёрнута

    g_mouse.lastX = xpos;
    g_mouse.lastY = ypos;
}

void scrollCallback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
    if (!g_scene) return;
    auto& cam = g_scene->getCamera();
    cam.fov -= static_cast<float>(yoffset) * 2.0f;
    // Clamp FOV в разумных пределах
    if (cam.fov < 10.0f)  cam.fov = 10.0f;
    if (cam.fov > 120.0f) cam.fov = 120.0f;
    cam.matricesDirty = true;
}

// -------------------------------------------------------
// Обработка клавиатуры (вызывается каждый кадр)
// -------------------------------------------------------
void processKeyboard(GLFWwindow* window, RenderEngine::Camera& camera, float dt)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(CAM_SPEED * dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveForward(-CAM_SPEED * dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveRight(-CAM_SPEED * dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(CAM_SPEED * dt);

    // Вертикаль: Space вверх, Ctrl/Shift вниз
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.moveUp(CAM_SPEED * dt);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.moveUp(-CAM_SPEED * dt);
}

// -------------------------------------------------------
// main
// -------------------------------------------------------
int main()
{
    // ---- GLFW ----
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
        "SCGL — FPS Camera", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Захватываем курсор сразу (FPS-режим)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // ---- GLAD ----
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
        return -1;
    }
    std::cout << "OpenGL  : " << glGetString(GL_VERSION) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";

    // ---- OpenGL state ----
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.12f, 0.12f, 0.14f, 1.0f);

    // ---- Шейдер ----
    auto shader = std::make_shared<RenderEngine::GLSLProgram>();
    try {
        shader->compileShader("shaders/vertexSimple.shader",
            RenderEngine::GLSLShader::VERTEX);
        shader->compileShader("shaders/fragSimple.shader",
            RenderEngine::GLSLShader::FRAGMENT);
        shader->link();
    }
    catch (const RenderEngine::GLSLProgramException& e) {
        std::cerr << "Shader error: " << e.what() << "\n";
        glfwTerminate();
        return -1;
    }

    // ---- Сцена ----
    Engine::Scene scene(SCR_WIDTH, SCR_HEIGHT);
    g_scene = &scene; // даём коллбэкам доступ

    // Стартовая позиция камеры
    scene.getCamera().setPosition(glm::vec3(0.0f, 1.0f, 6.0f));
    scene.getCamera().lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    // ---- Несколько кубов для сцены ----
    // Материалы с разными цветами
    auto matBlue = std::make_shared<RenderEngine::Material>(shader, "Blue");
    matBlue->setColor(glm::vec3(0.2f, 0.55f, 1.0f));

    auto matOrange = std::make_shared<RenderEngine::Material>(shader, "Orange");
    matOrange->setColor(glm::vec3(1.0f, 0.45f, 0.1f));

    auto matGreen = std::make_shared<RenderEngine::Material>(shader, "Green");
    matGreen->setColor(glm::vec3(0.25f, 0.8f, 0.35f));

    // Единый меш куба — переиспользуем для всех объектов
    auto cubeMesh = std::make_shared<RenderEngine::CubeMesh>("Cube");

    // Центральный куб
    {
        auto obj = std::make_shared<Engine::Object>("CenterCube");
        obj->setMesh(cubeMesh);
        obj->setMaterial(matBlue);
        obj->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        scene.addObject(obj);
    }
    // Правый куб
    {
        auto obj = std::make_shared<Engine::Object>("RightCube");
        obj->setMesh(cubeMesh);
        obj->setMaterial(matOrange);
        obj->transform.position = glm::vec3(3.0f, 0.5f, -1.0f);
        obj->transform.scale = glm::vec3(0.7f);
        scene.addObject(obj);
    }
    // Левый куб
    {
        auto obj = std::make_shared<Engine::Object>("LeftCube");
        obj->setMesh(cubeMesh);
        obj->setMaterial(matGreen);
        obj->transform.position = glm::vec3(-2.5f, -0.3f, 1.0f);
        obj->transform.scale = glm::vec3(1.2f);
        scene.addObject(obj);
    }
    // Задний куб поменьше
    {
        auto obj = std::make_shared<Engine::Object>("BackCube");
        obj->setMesh(cubeMesh);
        obj->setMaterial(matBlue);
        obj->transform.position = glm::vec3(0.5f, 0.0f, -4.0f);
        obj->transform.scale = glm::vec3(0.5f);
        scene.addObject(obj);
    }

    // ---- Game loop ----
    float lastTime = static_cast<float>(glfwGetTime());
    float angle = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = static_cast<float>(glfwGetTime());
        float dt = currentTime - lastTime;
        lastTime = currentTime;

        // Клавиатура
        processKeyboard(window, scene.getCamera(), dt);

        // Мышь — применяем накопленную дельту и сбрасываем
        if (g_mouse.yawDelta != 0.0f || g_mouse.pitchDelta != 0.0f) {
            scene.getCamera().rotate(g_mouse.yawDelta, g_mouse.pitchDelta);
            g_mouse.yawDelta = 0.0f;
            g_mouse.pitchDelta = 0.0f;
        }

        // Медленное вращение центрального куба
        angle += 45.0f * dt; // 45 градусов в секунду
        if (const auto& objs = scene.getObjects(); !objs.empty()) {
            objs[0]->transform.rotation =
                glm::quat(glm::vec3(glm::radians(angle * 0.4f),
                    glm::radians(angle),
                    0.0f));
        }

        scene.update(dt);
        scene.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    g_scene = nullptr;
    glfwTerminate();
    return 0;
}