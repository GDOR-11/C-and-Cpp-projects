//
//  2Drenderer.hpp
//  2Drenderer
//
//  Created by Gabriel Ramalho on 04/09/22.
//

#ifndef _Drenderer_hpp
#define _Drenderer_hpp

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define COLOR 0
#define TEXTURE 1

#warning use option+command+K on every function here lol

typedef struct Material {
    unsigned int type;
    union {
        glm::vec3 color;
        GLuint texture;
    };
    
    Material(glm::vec3 color = glm::vec3(0, 0, 0)) : color(color), type(COLOR) {}
    Material(GLuint texture) : texture(texture), type(TEXTURE) {}
    
} Material;

typedef struct Triangle {
    glm::vec2 A;
    glm::vec2 B;
    glm::vec2 C;
    
    Triangle(glm::vec2 A = glm::vec2(0), glm::vec2 B = glm::vec2(0), glm::vec2 C = glm::vec2(0)) : A(A), B(B), C(C) {};
} Triangle;

typedef struct Object {
    
    friend class Renderer;
    
private:
    GLuint vertexBuffer = -1; // exception case, i mean, i don't think there are going ot be 4294967295 fricking buffers
    
    int triangleCount;
    Triangle* triangles = NULL;
    
public:
    glm::vec2 position;
    glm::mat2 transform;
    Material* material;
    
    Object(glm::vec2 position = glm::vec2(0), glm::mat2 transform = glm::mat2(1), Material* material = new Material()) : position(position), transform(transform), material(material) {}
    
    
    /// see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml if you don't know what usage is
    void generateVertexBuffer(GLenum usage = GL_STATIC_DRAW);
    
#warning do this lul
    // TODO: implement this lul
    int getAmountOfTriangles() const;
    
    /// TODO: implement this lul
    Triangle* getTriangles() const;
    
    void setTriangles(std::vector<Triangle> triangles);
    void setTriangles(Triangle* triangles, int length);
    
    void setVertices(std::vector<glm::vec2> vertices);
    void setVertices(glm::vec2* vertices, int length);
    
} Object;

//typedef struct InstancedObject {
//
//    friend class Renderer;
//
//private:
//    GLint vertexBuffer = -1;
//
//    int triangleCount;
//    Triangle* triangles;
//
//    void generateVertexBuffer();
//
//public:
//
//    unsigned long instanceCount;
//    glm::vec2* positions;
//    glm::mat2* transforms;
//    Material* material;
//
//    InstancedObject(glm::vec2* positions, glm::mat2* transforms, Material* material, unsigned long instanceCount);
//    InstancedObject();
//
//    int getAmountOfTriangles();
//    Triangle* getTriangles();
//    void setTriangles(Triangle* triangles, unsigned int triangleCount);
//    void setTriangles(std::vector<Triangle> triangles);
//
//    void addInstance(glm::vec2 position, glm::mat2 transforms, Material material);
//
//} InstancedObject;



class Renderer {
#warning decide whats public and whats private in a reasonable way
#warning add keysPressed vector
    
private:
    GLuint positionUniformLocation;
    GLuint transformUniformLocation;
    GLuint windowSizeUniformLocation;
    
public:
    
    std::vector<Object*> objects;
//    std::vector<InstancedObject> instancedObjects;
    
    GLuint vertexArrayObject;
    GLuint program;
    
    int windowWidth;
    int windowHeight;
    
    GLFWwindow* window;
    
    
    
    /// initialize GLFW, GLFWwindow and GLEW and create the program
    bool initialize(unsigned int width, unsigned int height, const char* title, int antialiasing);
    
    // TODO: implement this lul
    void addObject(Object object);
    
//     TODO: https://stackoverflow.com/questions/41784790/how-to-update-vertex-buffer-data-frequently-every-frame-opengl
    
    /// just fricking render it. no need to explain. t h i s - r e n d e r s - t h e - s c e n e
    void render();
    
    /// returns an error message, if one was produced.
    const char* getInfoLog();
};

#endif /* _Drenderer_hpp */
