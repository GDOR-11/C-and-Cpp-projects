//
//  2Drenderer.cpp
//  2Drenderer
//
//  Created by Gabriel Ramalho on 04/09/22.
//

#include "2Drenderer.hpp"
#include "standardFragmentShader.frag"
#include "standardVertexShader.vert"
#include "earcut.hpp"
#include <array>

/// description of any error that may have happened
std::string infoLog;
/// is there an error?
bool error = false;



void Object::generateVertexBuffer(GLenum usage) {
    glGenBuffers(1, &this->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * triangleCount, triangles, usage);
}

inline int Object::getAmountOfTriangles() const {
    return triangleCount;
}

Triangle* Object::getTriangles() const {
    Triangle* triangles_copy = new Triangle[triangleCount];
    return (Triangle*) memcpy(triangles_copy, triangles, sizeof(Triangle) * triangleCount);
}


void Object::setTriangles(std::vector<Triangle> triangles) {
    this->setTriangles(triangles.data(), (int) triangles.size());
}
void Object::setTriangles(Triangle* triangles, int length) {
    this->triangleCount = length;
    delete[] this->triangles;
    this->triangles = new Triangle[this->triangleCount];
    memcpy(this->triangles, triangles, length);
}

void Object::setVertices(std::vector<glm::vec2> vertices) {
    this->setVertices(vertices.data(), (int) vertices.size());
}
void Object::setVertices(glm::vec2* vertices, int length) {
    auto convertVec2toArray = [](glm::vec2 vec2) {
        return (std::array<double, 2>) {vec2.x, vec2.y};
    };
//    std::vector<std::vector<std::array<double, 2>>> polygon = {std::vector<std::array<double, 2>>(length, {0, 0})};
//    std::transform(vertices, vertices + length, std::back_inserter(polygon[0]), convertVec2toArray);
    std::vector<std::vector<glm::vec2>> polygon = {std::vector<glm::vec2>()};
    polygon[0].assign(vertices, vertices + length);
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);
    
    this->triangleCount = (int) indices.size() / 3;
    
    delete[] triangles;
    triangles = new Triangle[this->triangleCount];
    for(int i = 0;i < triangleCount;i++) {
        triangles[i] = Triangle(vertices[indices[3 * i]], vertices[indices[3 * i + 1]], vertices[indices[3 * i + 2]]);
    }
}




/// pretty self explanatory
GLFWwindow* createGLFWwindow(unsigned int width, unsigned int height, const char* title, int antialiasing) {
    
    glfwWindowHint(GLFW_SAMPLES, antialiasing);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // to make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // no old OpenGL
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    
    if(window == NULL) {
        infoLog = (char*) "Failed to open GLFW window.";
    }
    return window;
}

/// just compile the fricking shader
bool compileShader(GLuint shaderID, const char* shaderCode, GLenum shaderType) {
    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        int compilingErrorInfoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &compilingErrorInfoLogLength);
        char* compilingErrorInfoLog = new char[compilingErrorInfoLogLength];
        glGetShaderInfoLog(shaderID, compilingErrorInfoLogLength, NULL, compilingErrorInfoLog);
        infoLog = shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment";
        infoLog += " shader compiling error:\n\n\n";
        infoLog += compilingErrorInfoLog;
        infoLog += "\n\n\nShader code:\n\n";
        infoLog += shaderCode;
    }
    return success;
}

/// just link the fricking program
bool linkProgram(GLuint programID, GLuint vertexShaderID, GLuint fragmentShaderID) {
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if(!success) {
        int linkingErrorInfoLogLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &linkingErrorInfoLogLength);
        char* linkingErrorInfoLog = new char[linkingErrorInfoLogLength];
        glGetProgramInfoLog(programID, linkingErrorInfoLogLength, NULL, linkingErrorInfoLog);
        infoLog = "Program linking error:\n\n\n";
        infoLog += linkingErrorInfoLog;
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    return success;
}

/// create, compile and link the program
GLuint createProgram(const char* vertexShaderCode, const char* fragmentShaderCode) {
    
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint programID = glCreateProgram();
    
    error = !(compileShader(vertexShaderID, vertexShaderCode, GL_VERTEX_SHADER) && compileShader(fragmentShaderID, fragmentShaderCode, GL_FRAGMENT_SHADER) && linkProgram(programID, vertexShaderID, fragmentShaderID));
    
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    
    return programID;
}

bool Renderer::initialize(unsigned int width, unsigned int height, const char* title, int antialiasing) {
    
    if(!glfwInit()) {
        infoLog = (char*) "Failed to initialize GLFW.";
        return false;
    }
    
    
    window = createGLFWwindow(width, height, title, antialiasing);
    
    if(window == NULL) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    
    windowWidth = width;
    windowHeight = height;
    
    
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        infoLog = (char*) "Failed to initialize GLEW.";
        return false;
    }
    
    
    program = createProgram(vertexShaderCode, fragmentShaderCode);
    if(error) {
        return false;
    }
    
    positionUniformLocation = glGetUniformLocation(program, "objectPosition");
    transformUniformLocation = glGetUniformLocation(program, "transformMatrix");
    
    
    glGenVertexArrays(1, &vertexArrayObject);
    
    return true;
}


/// just fricking render it. no need to explain. t h i s - r e n d e r s - t h e - s c e n e
void Renderer::render() {
    glClearColor(0, 0, 0, 1);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    
    // didn't think it was worth it to take up more memory just to save a single glGetUniformLocation call
    glUniform2f(glGetUniformLocation(program, "windowSize"), windowWidth, windowHeight);
    
    glUseProgram(program);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArray(vertexArrayObject);
    
    for(int i = 0;i < objects.size();i++) {
        
        glUniform2f(positionUniformLocation, objects[i]->position.x, objects[i]->position.y);
        glUniformMatrix2fv(transformUniformLocation, 1, GL_FALSE, &objects[i]->transform[0][0]);
        
        
        glBindBuffer(GL_ARRAY_BUFFER, objects[i]->vertexBuffer);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);
        
        glEnableVertexAttribArray(0);
            
        glDrawArrays(GL_TRIANGLES, 0, 3 * objects[i]->triangleCount);
        
        glDisableVertexAttribArray(0);
    }
    
    // for later on: glDrawArraysInstanced();
//    for(int i = 0;i < instancedObjects.size();i++) {
//        glBindBuffer(GL_ARRAY_BUFFER, objects[i].vertexBuffer);
//        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);
//
//        glEnableVertexAttribArray(0);
//
//        glDrawArrays(GL_TRIANGLES, 0, 3 * objects[i].triangleCount);
//
//        glDisableVertexAttribArray(0);
//    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}

const char* Renderer::getInfoLog() {
    return infoLog.c_str();
}
