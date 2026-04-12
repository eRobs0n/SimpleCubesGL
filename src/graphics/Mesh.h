#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <Material.h>
namespace RenderEngine {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 UV;
    };

	class Mesh {
    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        unsigned int VAO = 0;
        unsigned int VBO = 0;
        unsigned int IBO = 0;
        std::string name;               
        bool indexed = false;
        void setupMesh();
    public:
        Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds, const std::string& name = "UnnamedMesh");
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(Mesh&& other) noexcept;

        void bind() const;
        void unbind() const;
        bool isIndexed() {
            return indexed;
        }
        int getIndexCount() const { return (GLsizei)indices.size(); }
        int getVertexCount() const { return (GLsizei)vertices.size(); }
        ~Mesh();

	};
    class CubeMesh : public Mesh
    {
    public:
        CubeMesh(const std::string& name = "Cube")
            : Mesh({}, {}, name)
        {
            generateCube();
        }

    private:
        void generateCube();
    };

}