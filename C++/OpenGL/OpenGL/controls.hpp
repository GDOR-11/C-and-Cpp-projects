#ifndef CONTROLS_HPP
#define CONTROLS_HPP

//void computeMatricesFromInputs();
//glm::mat4 getViewMatrix();
//glm::mat4 getProjectionMatrix();

// v2 of my code
class ControlableCamera {

private:
    glm::vec3 cameraViewDirection;
    glm::vec3 cameraRightVector;
    glm::vec3 cameraUpVector;

    float lastFrameTime;
    double lastMouseX;
    double lastMouseY;

    bool cursorFocused = false;

    void calculateOrientationVectors();

public:
    float movementSpeed = 3;
    float rotationSpeed = 0.05;
    float FOV = 60;
    float aspectRatio = 4.0 / 3;
    float nearClipPlane = 0.1;
    float farClipPlane = 100;

    float minYrotation = 1e-3; // an error margin is recommended for these specific values
    float maxYrotation = 3.1405;

    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec2 rotation = glm::vec2(0, 0);

    void lookAt(glm::vec3 point);
    void processInputs(GLFWwindow* window);
    glm::mat4 calculateMVPmatrix(glm::mat4 modelMatrix);
};

#endif
