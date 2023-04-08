//
//  3Drenderer.hpp
//  tutorial10_transparency
//
//  Created by Gabriel Ramalho on 14/06/22.
//

#ifndef _Drenderer_hpp
#define _Drenderer_hpp

#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "renderer.hpp"
//class Object : OpenGLentity {
//
//private:
//    std::vector<GLuint> buffers;
//
//    GLuint texture;
//
//public:
//    Object(std::vector<glm::vec3> vertexPositions, std::vector<glm::vec2> vertexUVs, std::vector<glm::vec3> vertexNormals, std::vector<unsigned int> vertexIndices, GLuint textureID);
//};

#define AMBIENT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define POINT_LIGHT 2

typedef struct {
    unsigned int type;
    glm::vec3 position;
	glm::vec3 direction;
    glm::vec3 color;
    float intensity;
} LightSource;


class Renderer {
    
//private:
public:
    std::vector<LightSource> lights;
    
    std::vector<GLuint> buffers;
    std::vector<unsigned int> vertexCount;
    std::vector<GLuint> textures;
    std::vector<float> specularLightingIntensity;
    std::vector<float> shininess;
	
    GLuint vertexArrayObject;
    GLuint program;
    
    glm::vec3 cameraPosition;
    glm::vec2 cameraRotation;
    
    std::vector<glm::mat4> modelMatrices;
    
    
    int windowWidth;
    int windowHeight;
    
    glm::vec3 cameraViewDirection;
    glm::vec3 cameraRightVector;
    glm::vec3 cameraUpVector;
    glm::vec3 forwardMovementDirection;
    glm::vec3 rightMovementDirection;
    
    void calculateCameraOrientationVectors();
    glm::mat4 calculateProjectionMatrix();
    glm::mat4 calculateViewMatrix();
    
//public:

    float FOV;
    float nearClipPlane;
    float farClipPlane;
    
    glm::vec3 backgroundColor;
    
    GLFWwindow* window;
    bool init(int width, int height, const char* title, int antialiasing, int openglMajorVersion, int openglMinorVersion);
    bool processShaders(const char* vertexShaderFilePath = NULL, const char* fragmentShaderFilePath = NULL);
    
    
    void render();
    
    void createObject(GLuint vertexBuffer, GLuint UVbuffer, GLuint normalBuffer, GLuint elementBuffer, unsigned int vertexCount, GLuint texture, float specularLightingIntensity, float shininess);
    
    void resize(int width, int height);
    int getWindowWidth();
    int getWindowHeight();
    
    void setVertexArrayObject(GLuint vertexArrayObject);
    
    std::string getInfoLog();
    
};

#endif /* _Drenderer_hpp */
