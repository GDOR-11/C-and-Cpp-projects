//
//  demo.cpp
//  2Drenderer
//
//  Created by Gabriel Ramalho on 04/09/22.
//

#include <iostream>

#include "2Drenderer.hpp"
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, const char * argv[]) {
    
    
    Renderer renderer;
    if(!renderer.initialize(1024, 728, "2Drenderer demo", 4)) {
        std::cout << renderer.getInfoLog() << "\n";
        return -1;
    }
    
//    std::vector<Triangle> triangles;
//    triangles.push_back(Triangle(glm::vec2(-50, -50), glm::vec2(-50, 50), glm::vec2(50, -50)));
//    triangles.push_back(Triangle(glm::vec2(50, 50), glm::vec2(-50, 50), glm::vec2(50, -50)));
    
    Object* object = new Object();
    object->position = glm::vec2(512, 364);
//    object->setTriangles(triangles);
    std::vector<glm::vec2> vertices;
    vertices.push_back(glm::vec2(-50, -50));
    vertices.push_back(glm::vec2(-50, 50));
    vertices.push_back(glm::vec2(50, 50));
    vertices.push_back(glm::vec2(50, -50));
    object->setVertices(vertices);
    object->generateVertexBuffer();

    renderer.objects.push_back(object);
    do {
        #define SPEED 10
        if(glfwGetKey(renderer.window, GLFW_KEY_W) == GLFW_PRESS)
            object->position.y -= SPEED;
        if(glfwGetKey(renderer.window, GLFW_KEY_A) == GLFW_PRESS)
            object->position.x -= SPEED;
        if(glfwGetKey(renderer.window, GLFW_KEY_S) == GLFW_PRESS)
            object->position.y += SPEED;
        if(glfwGetKey(renderer.window, GLFW_KEY_D) == GLFW_PRESS)
            object->position.x += SPEED;
        renderer.render();

    } while(!glfwWindowShouldClose(renderer.window));

    return 0;
}
