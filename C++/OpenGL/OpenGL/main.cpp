//
//  main.cpp
//  OpenGL
//
//  Created by Gabriel Ramalho on 02/08/22.
//

#include <iostream>
#include <vector>


// GLEW
// note: always include glew.h before gl.h an glfw3.h
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#if _WIN32 || _WIN64
    #include <Windows.h>
#else
    #include <unistd.h>
    #define Sleep(x) usleep(1000 * x)
#endif

//#include <common/shader.hpp>
#include "texture.hpp"
#include "controls.hpp"
#include "objloader.hpp"
//#include <common/renderer.hpp>
#include "3Drenderer.hpp"

unsigned int FPS = 60;
int width = 1024;
int height = 728;


GLuint createBuffer(GLenum bufferType, const void* bufferData, unsigned long bufferSize, GLenum usage) {
    // see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(bufferType, buffer);
    glBufferData(bufferType, bufferSize, bufferData, usage);
    return buffer;
}

void setLightUniforms(GLint programID, glm::vec3 lightPosition, glm::vec3 lightColor, float diffuseLightIntensity, float ambientLightIntensity, float specularLightIntensity, float specularLightDistribution) {
    // just a bunch of glGetUniformLocation and glUniformXf
    GLuint lightPositionLocation = glGetUniformLocation(programID, "lightPosition");
    glUniform3f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z);
    GLuint lightColorLocation = glGetUniformLocation(programID, "lightColor");
    glUniform3f(lightColorLocation, lightColor.r, lightColor.g, lightColor.b);
    GLuint diffuseLightIntensityLocation = glGetUniformLocation(programID, "diffuseLightIntensity");
    glUniform1f(diffuseLightIntensityLocation, diffuseLightIntensity);
    GLuint ambientLightIntensityLocation = glGetUniformLocation(programID, "ambientLightIntensity");
    glUniform1f(ambientLightIntensityLocation, ambientLightIntensity);
    GLuint specularLightIntensityLocation = glGetUniformLocation(programID, "specularLightIntensity");
    glUniform1f(specularLightIntensityLocation, specularLightIntensity);
    GLuint specularLightDistributionLocation = glGetUniformLocation(programID, "specularLightDistribution");
    glUniform1f(specularLightDistributionLocation, specularLightDistribution);
}

int main(int argc, const char* argv[]) {
    Renderer renderer;
    if(!renderer.init(1024, 728, "OpenGL test", 4, 3, 3)) {
        std::cerr << renderer.getInfoLog() << "\n";
        return -1;
    }
	
	if(!renderer.processShaders()) {
        std::cerr << renderer.getInfoLog() << "\n";
        return -1;
    }


    
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    renderer.setVertexArrayObject(vertexArrayObject);



    std::vector<glm::vec3> vertexPositions;
    std::vector<glm::vec2> vertexUVs;
    std::vector<glm::vec3> vertexNormals;
    std::vector<unsigned int> indices;
    loadOBJobject("suzanne.obj", indices, vertexPositions, vertexUVs, vertexNormals);

    GLuint vertexBuffer = createBuffer(GL_ARRAY_BUFFER, &vertexPositions[0], vertexPositions.size() * sizeof(glm::vec3), GL_STATIC_DRAW);
    GLuint UVbuffer = createBuffer(GL_ARRAY_BUFFER, &vertexUVs[0], vertexUVs.size() * sizeof(glm::vec2), GL_STATIC_DRAW);
    GLuint normalBuffer = createBuffer(GL_ARRAY_BUFFER, &vertexNormals[0], vertexNormals.size() * sizeof(glm::vec3), GL_STATIC_DRAW);
    GLuint elementBuffer = createBuffer(GL_ELEMENT_ARRAY_BUFFER, &indices[0], indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);
    
    std::vector<glm::vec3> vertexPositions2;
    std::vector<glm::vec2> vertexUVs2;
    std::vector<glm::vec3> vertexNormals2;
    std::vector<unsigned int> indices2;
    loadOBJobject("cube.obj", indices2, vertexPositions2, vertexUVs2, vertexNormals2);

    GLuint vertexBuffer2 = createBuffer(GL_ARRAY_BUFFER, &vertexPositions2[0], vertexPositions2.size() * sizeof(glm::vec3), GL_STATIC_DRAW);
    GLuint UVbuffer2 = createBuffer(GL_ARRAY_BUFFER, &vertexUVs2[0], vertexUVs2.size() * sizeof(glm::vec2), GL_STATIC_DRAW);
    GLuint normalBuffer2 = createBuffer(GL_ARRAY_BUFFER, &vertexNormals2[0], vertexNormals2.size() * sizeof(glm::vec3), GL_STATIC_DRAW);
    GLuint elementBuffer2 = createBuffer(GL_ELEMENT_ARRAY_BUFFER, &indices2[0], indices2.size() * sizeof(unsigned int), GL_STATIC_DRAW);
    
    

    bool success;
    GLuint textureID = createTexture(DDS, "uvmap.DDS", &success);
    if(!success) {
        return -1;
    }
    renderer.textures.push_back(textureID);

    
    renderer.FOV = 60;
    renderer.nearClipPlane = 0.1;
    renderer.farClipPlane = 100;
    renderer.backgroundColor = glm::vec3(0, 0, 0);

    glfwSetInputMode(renderer.window, GLFW_STICKY_KEYS, GL_TRUE);
    
    renderer.createObject(vertexBuffer, UVbuffer, normalBuffer, elementBuffer, (unsigned int) indices.size(), textureID, 0.75, 10);
    renderer.createObject(vertexBuffer2, UVbuffer2, normalBuffer2, elementBuffer2, (unsigned int) indices2.size(), textureID, 0.25, 10);
    renderer.modelMatrices.push_back(glm::translate(glm::mat4(1), glm::vec3(-1.5, 0, 0)));
	renderer.modelMatrices.push_back(glm::translate(glm::mat4(1), glm::vec3(1.5, 0, 0)));
    
    LightSource light = {AMBIENT_LIGHT, glm::vec3(3, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 0.2};
    renderer.lights.push_back(light);
    LightSource light2 = {DIRECTIONAL_LIGHT, glm::vec3(0, 0, 0), glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), 0.5};
    renderer.lights.push_back(light2);
    LightSource light3 = {POINT_LIGHT, glm::vec3(-1, 2, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 1};
    renderer.lights.push_back(light3);
    
	
//	glfwSwapInterval(0); // ONLY USE THIS FOR MEASURING PERFORMANCE. IT USES 100% GPU POWER
	
    ControlableCamera camera;
    double simTime = 0;
	double lastTime = glfwGetTime();
    do {
		double time = glfwGetTime();
        if(glfwGetKey(renderer.window, GLFW_KEY_SPACE) != GLFW_PRESS) {
			camera.position = glm::vec3(7.5 * sin(simTime / 1.5), 0, 7.5 * cos(simTime / 1.5));
			simTime += time - lastTime;
        }
        camera.lookAt(glm::vec3(0, 0, 0));
        renderer.cameraPosition = camera.position;
        renderer.cameraRotation = camera.rotation;
        renderer.render();
		
		lastTime = time;
    } while(!glfwWindowShouldClose(renderer.window));

//    glDeleteBuffers(1, &elementBuffer);
//    glDeleteBuffers(1, &vertexBuffer);
//    glDeleteBuffers(1, &UVbuffer);
//    glDeleteBuffers(1, &normalBuffer);
//    glDeleteProgram(programID);
//    glDeleteTextures(1, &textureID);
//    glDeleteVertexArrays(1, &vertexArrayObjectID);

    glfwTerminate();

    return 0;
}
