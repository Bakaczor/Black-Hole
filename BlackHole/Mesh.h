#pragma once

// THIS CLASS IS BASED ON: https://learnopengl.com/Model-Loading/Mesh

#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    // constant shading
    glm::vec3 ConstPosition;
    glm::vec3 ConstNormal;
    glm::vec2 ConstTexCoords;
    int index;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    // render the mesh
    void Draw(Shader& shader);

    private:
    // render data 
    unsigned int VAO, VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh();
};
#endif