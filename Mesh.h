#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include"Shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 texCoords;
    
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
  
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh
{
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void draw(Shader& shader);

    private:
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        unsigned int VAO;
        unsigned int VBO, EBO;

        void setupMesh();
};
#endif
