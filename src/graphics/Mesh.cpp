#include "Mesh.h"
#include <glad/glad.h>
namespace RenderEngine {
    Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds, const std::string& meshName)
        : vertices(verts), indices(inds), name(meshName)
    {
        setupMesh();
    }
    void Mesh::setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        if (!indices.empty())
        {
            indexed = true;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                indices.data(), GL_STATIC_DRAW);
        }

        // Атрибуты вершин
        // location 0: position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

        // location 1: normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // location 2: texCoords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));

        glBindVertexArray(0);
    }
    void Mesh::bind() const {
        glBindVertexArray(VAO);
    }
    void Mesh::unbind() const {
        glBindVertexArray(0);
    }
    Mesh::~Mesh()
    {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (IBO) glDeleteBuffers(1, &IBO);
    }
    Mesh::Mesh(Mesh&& other) noexcept
    {
        *this = std::move(other);
    }

    Mesh& Mesh::operator=(Mesh&& other) noexcept
    {
        if (this != &other)
        {
            // Освобождаем текущие ресурсы
            if (VAO) glDeleteVertexArrays(1, &VAO);
            if (VBO) glDeleteBuffers(1, &VBO);
            if (IBO) glDeleteBuffers(1, &IBO);

            vertices = std::move(other.vertices);
            indices = std::move(other.indices);
            name = std::move(other.name);

            VAO = other.VAO;
            VBO = other.VBO;
            IBO = other.IBO;
            indexed = other.indexed;
            other.indexed = false;
            other.VAO = other.VBO = other.IBO = 0;
        }
        return *this;
    }

    void CubeMesh::generateCube()
    {
        vertices = {
            // ===== FRONT (+Z) =====
            {{-0.5f, -0.5f,  0.5f}, {0, 0, 1}, {0, 0}},
            {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}, {1, 0}},
            {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1, 1}},
            {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}, {0, 1}},

            // ===== BACK (-Z) =====
            {{ 0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0}},
            {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1, 0}},
            {{-0.5f,  0.5f, -0.5f}, {0, 0, -1}, {1, 1}},
            {{ 0.5f,  0.5f, -0.5f}, {0, 0, -1}, {0, 1}},

            // ===== RIGHT (+X) =====
            {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}, {0, 0}},
            {{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}, {1, 0}},
            {{ 0.5f,  0.5f, -0.5f}, {1, 0, 0}, {1, 1}},
            {{ 0.5f,  0.5f,  0.5f}, {1, 0, 0}, {0, 1}},

            // ===== LEFT (-X) =====
            {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0, 0}},
            {{-0.5f, -0.5f,  0.5f}, {-1, 0, 0}, {1, 0}},
            {{-0.5f,  0.5f,  0.5f}, {-1, 0, 0}, {1, 1}},
            {{-0.5f,  0.5f, -0.5f}, {-1, 0, 0}, {0, 1}},

            // ===== TOP (+Y) =====
            {{-0.5f,  0.5f,  0.5f}, {0, 1, 0}, {0, 0}},
            {{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}, {1, 0}},
            {{ 0.5f,  0.5f, -0.5f}, {0, 1, 0}, {1, 1}},
            {{-0.5f,  0.5f, -0.5f}, {0, 1, 0}, {0, 1}},

            // ===== BOTTOM (-Y) =====
            {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0, 0}},
            {{ 0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1, 0}},
            {{ 0.5f, -0.5f,  0.5f}, {0, -1, 0}, {1, 1}},
            {{-0.5f, -0.5f,  0.5f}, {0, -1, 0}, {0, 1}},
        };

        indices = {
            0, 1, 2,   2, 3, 0,       // front
            4, 5, 6,   6, 7, 4,       // back
            8, 9,10,  10,11, 8,       // right
           12,13,14,  14,15,12,       // left
           16,17,18,  18,19,16,       // top
           20,21,22,  22,23,20        // bottom
        };
    }

}