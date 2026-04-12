cmake_minimum_required(VERSION 3.20)
project(SimpleCubesGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------- OpenGL (ДО зависимостей) ----------------
find_package(OpenGL REQUIRED)

include(FetchContent)

# ---------------- GLFW ----------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)

# ---------------- GLM ----------------
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# ---------------- GLAD ----------------
add_library(glad STATIC
    external/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    external/glad/include
)

# ---------------- Engine Library ----------------
add_library(scgl_engine STATIC
    src/engine/Object.cpp
    src/engine/Scene.cpp
    src/graphics/shader.cpp
    src/graphics/Mesh.cpp
    src/graphics/Material.cpp
    src/graphics/Camera.cpp
    src/graphics/Renderer.cpp
)

target_include_directories(scgl_engine PUBLIC
    src
    src/engine
    src/graphics
    external/stb
)

target_link_libraries(scgl_engine PUBLIC
    glad
    glfw
    glm::glm
    OpenGL::GL
)

# ---------------- Executable ----------------
add_executable(SimpleCubesGL
    src/main.cpp
)

target_link_libraries(SimpleCubesGL PRIVATE
    scgl_engine
)

# Копируем шейдеры рядом с исполняемым файлом
add_custom_command(TARGET SimpleCubesGL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/shaders
        $<TARGET_FILE_DIR:SimpleCubesGL>/shaders
)