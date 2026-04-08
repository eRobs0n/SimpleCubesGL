#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
const char* vertexShaderSource = R"(
// ===== VERTEX SHADER =====
#version 330 core

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 TexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

uniform mat4 MVP;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;

void main() {
    FragPos = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
    Normal  = NormalMatrix * VertexNormal; // нормализуем во фрагментном
    UV      = TexCoord;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}
)";

const char* fragmentShaderSource = R"(
// ===== FRAGMENT SHADER =====
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform sampler2D tex;

uniform vec3  LightPos;    // в view space
uniform vec3  LightColor;
uniform float AmbientStrength;   // 0.0 .. 1.0
uniform float SpecularStrength;  // 0.0 .. 1.0
uniform float Shininess;         // 2, 4, 8, 16, 32 ...

void main() {
    vec3 texColor = texture(tex, UV).rgb;

    // --- нормали ---
    vec3 norm     = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    vec3 viewDir  = normalize(-FragPos); // camera at origin in view space

    // --- ambient ---
    vec3 ambient = AmbientStrength * LightColor;

    // --- diffuse ---
    float diff    = max(dot(norm, lightDir), 0.0);
    vec3  diffuse = diff * LightColor;

    // --- specular (Blinn-Phong) ---
    vec3  halfDir  = normalize(lightDir + viewDir);
    float spec     = pow(max(dot(norm, halfDir), 0.0), Shininess);
    vec3  specular = SpecularStrength * spec * LightColor;

    // --- attenuation ---
    float dist        = length(LightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);

    // --- сборка ---
    vec3 result = (ambient + (diffuse + specular) * attenuation) * texColor;

    // --- гамма-коррекция ---
    result = pow(result, vec3(1.0 / 2.2));

    FragColor = vec4(result, 1.0);
}
)";




float vertices[] = {

    // ===== FRONT =====
    -0.5f,-0.5f, 0.5f,   0,0,1,   0,0,
     0.5f,-0.5f, 0.5f,   0,0,1,   1,0,
     0.5f, 0.5f, 0.5f,   0,0,1,   1,1,
     0.5f, 0.5f, 0.5f,   0,0,1,   1,1,
    -0.5f, 0.5f, 0.5f,   0,0,1,   0,1,
    -0.5f,-0.5f, 0.5f,   0,0,1,   0,0,

    // ===== BACK =====
    -0.5f,-0.5f,-0.5f,   0,0,-1,  1,0,
     0.5f,-0.5f,-0.5f,   0,0,-1,  0,0,
     0.5f, 0.5f,-0.5f,   0,0,-1,  0,1,
     0.5f, 0.5f,-0.5f,   0,0,-1,  0,1,
    -0.5f, 0.5f,-0.5f,   0,0,-1,  1,1,
    -0.5f,-0.5f,-0.5f,   0,0,-1,  1,0,

    // ===== LEFT =====
    -0.5f, 0.5f, 0.5f,  -1,0,0,   1,1,
    -0.5f, 0.5f,-0.5f,  -1,0,0,   0,1,
    -0.5f,-0.5f,-0.5f,  -1,0,0,   0,0,
    -0.5f,-0.5f,-0.5f,  -1,0,0,   0,0,
    -0.5f,-0.5f, 0.5f,  -1,0,0,   1,0,
    -0.5f, 0.5f, 0.5f,  -1,0,0,   1,1,

    // ===== RIGHT =====
     0.5f, 0.5f, 0.5f,   1,0,0,   0,1,
     0.5f, 0.5f,-0.5f,   1,0,0,   1,1,
     0.5f,-0.5f,-0.5f,   1,0,0,   1,0,
     0.5f,-0.5f,-0.5f,   1,0,0,   1,0,
     0.5f,-0.5f, 0.5f,   1,0,0,   0,0,
     0.5f, 0.5f, 0.5f,   1,0,0,   0,1,

     // ===== BOTTOM =====
     -0.5f,-0.5f,-0.5f,   0,-1,0,  0,1,
      0.5f,-0.5f,-0.5f,   0,-1,0,  1,1,
      0.5f,-0.5f, 0.5f,   0,-1,0,  1,0,
      0.5f,-0.5f, 0.5f,   0,-1,0,  1,0,
     -0.5f,-0.5f, 0.5f,   0,-1,0,  0,0,
     -0.5f,-0.5f,-0.5f,   0,-1,0,  0,1,

     // ===== TOP =====
     -0.5f, 0.5f,-0.5f,   0,1,0,   0,1,
      0.5f, 0.5f,-0.5f,   0,1,0,   1,1,
      0.5f, 0.5f, 0.5f,   0,1,0,   1,0,
      0.5f, 0.5f, 0.5f,   0,1,0,   1,0,
     -0.5f, 0.5f, 0.5f,   0,1,0,   0,0,
     -0.5f, 0.5f,-0.5f,   0,1,0,   0,1
};
float cubeEdges[] = {
    // нижний квадрат
    -0.5f,-0.5f,-0.5f,   0.5f,-0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,   0.5f,-0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,  -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,  -0.5f,-0.5f,-0.5f,

    // верхний квадрат
    -0.5f, 0.5f,-0.5f,   0.5f, 0.5f,-0.5f,
     0.5f, 0.5f,-0.5f,   0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,  -0.5f, 0.5f,-0.5f,

    // вертикали
    -0.5f,-0.5f,-0.5f,  -0.5f, 0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,   0.5f, 0.5f,-0.5f,
     0.5f,-0.5f, 0.5f,   0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,  -0.5f, 0.5f, 0.5f
};

const int CHUNK_SIZE = 16;

enum BlockType {
    AIR = 0,
    DIRT
};

struct Chunk {
    BlockType blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

    BlockType get(int x, int y, int z) {
        return blocks[x][y][z];
    }

    void set(int x, int y, int z, BlockType type) {
        blocks[x][y][z] = type;
    }
};

float yaw = -90.0f;   // влево/вправо
float pitch = 0.0f;   // вверх/вниз

float lastX = 400, lastY = 300;
bool firstMouse = true;

float sensitivity = 0.1f;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // инверсия Y

    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // ограничение (чтобы не переворачивалась)
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
}


int main() {
    std::cout << "Window is NULL\n";
    if (!glfwInit()) {
        std::cout << "GLFW init failed\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1000, 800, "SimpleCubesGL  ", nullptr, nullptr);
    if (!window) {
        std::cout << "Window is NULL\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD init failed\n";
        return -1;
    }
    glfwSetCursorPosCallback(window, mouse_callback);
   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);




    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // позиция
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // нормаль
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // настройки
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    std::cout << "Window is NULL\n";
    // загрузка
    int width, height, channels;
    unsigned char* data = stbi_load("D:/block.png", &width, &height, &channels, 4); // принудительно RGBA
    std::cout << "СL\n";


    std::cout << "СL\n";

    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // отключить выравнивание

        std::cout << "Window is NULL\n";
        glTexImage2D(GL_TEXTURE_2D, 0, format,
            width, height, 0,
            format, GL_UNSIGNED_BYTE, data);
        std::cout << "Window is NULL\n";
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture\n";
    }

    stbi_image_free(data);
    std::cout << "Window is NULL\n";
    Chunk chunk;

    // генерация (плоская земля)
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {

                if (y < 3)
                    chunk.set(x, y, z, DIRT);
                else
                    chunk.set(x, y, z, AIR);
            }
        }
    }

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::vec3 cameraPos = glm::vec3(8.0f, 5.0f, 20.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float cameraSpeed = 0.1f;
    std::cout << "СL\n";

    while (!glfwWindowShouldClose(window)) {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            cameraPos.y += cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            cameraPos.y -= cameraSpeed;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform3f(glGetUniformLocation(shaderProgram, "color"),
            0.4f, 0.8f, 0.3f);

        glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"),
            0.6f);

        // кешируем локации
        int MVP_loc = glGetUniformLocation(shaderProgram, "MVP");
        glBindVertexArray(VAO);

        glm::mat4 view = glm::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            cameraUp
        );

        // позиция света (в мире)
        glm::vec3 lightPos = glm::vec3(8.0f, 10.0f, 8.0f);

        // переводим в view space
        glm::vec3 lightPosView = glm::vec3(view * glm::vec4(lightPos, 1.0));

        glUniform3f(glGetUniformLocation(shaderProgram, "LightPos"),
            lightPosView.x, lightPosView.y, lightPosView.z);

        // цвет света
        glUniform3f(glGetUniformLocation(shaderProgram, "LightColor"),
            1.0f, 1.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1f(glGetUniformLocation(shaderProgram, "AmbientStrength"), 0.15f);
        glUniform1f(glGetUniformLocation(shaderProgram, "SpecularStrength"), 0.3f);
        glUniform1f(glGetUniformLocation(shaderProgram, "Shininess"), 16.0f);
        glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 0);
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {

                    if (chunk.get(x, y, z) == AIR)
                        continue;

                    glm::mat4 model = glm::translate(
                        glm::mat4(1.0f),
                        glm::vec3(x, y, z)
                    );

                    // матрицы
                    glm::mat4 modelView = view * model;
                    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelView)));
                    glm::mat4 mvp = projection * view * model;

                    // передаём
                    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"),
                        1, GL_FALSE, &mvp[0][0]);

                    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "ModelViewMatrix"),
                        1, GL_FALSE, &modelView[0][0]);

                    glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "NormalMatrix"),
                        1, GL_FALSE, &normalMatrix[0][0]);

                    // цвет блока (можно варьировать)
                    glUniform3f(glGetUniformLocation(shaderProgram, "ObjectColor"),
                        0.6f, 0.8f, 0.4f);

                    // рисуем
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }
       

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}