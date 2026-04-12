cmake_minimum_required(VERSION 3.20)
project(SimpleCubesGL)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# ---------------- GLFW ----------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# ---------------- GLM ----------------
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# ---------------- GLAD ----------------
add_library(glad
    external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    external/glad/include
)

# ---------------- Executable ----------------
add_executable(SimpleCubesGL
    src/main.cpp
    src/graphics/shader.cpp
)

target_include_directories(SimpleCubesGL PRIVATE
    external/stb
)

target_link_libraries(SimpleCubesGL PRIVATE
    glfw
    glad
    glm::glm
    OpenGL::GL
)

# ---------------- OpenGL ----------------
find_package(OpenGL REQUIRED)