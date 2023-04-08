//
//  3Drenderer.cpp
//  tutorial10_transparency
//
//  Created by Gabriel Ramalho on 14/06/22.
//

#include "3Drenderer.hpp"

#include <filesystem>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define NORMAL_MATRIX(mat) (glm::transpose(glm::inverse((mat))))

std::string infoLog;
bool error = false;

const char* readFile(const char* path) {
    std::ifstream file(path);
    char* content;
    if(!file.is_open()) {
		if(path == NULL) {
			infoLog = "Provided path is NULL\n";
			return nullptr;
		}
        infoLog = "Could not open file ";
        infoLog += path;
        infoLog += ".\n";
        return nullptr;
    }

    file.seekg(0, std::ios::end); // go to the end
    long length = file.tellg();    // get location (which is the length)
    file.seekg(0, std::ios::beg); // go back to the beginning

    content = new char[length + 1];
    file.read(content, length);

    content[length] = '\0';

    file.close();

    return content;
}

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
        infoLog += "\n\n\n";
    }
    return success;
}

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
        infoLog += "\n\n\n";
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    return success;
}

GLuint createProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) {
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint programID = glCreateProgram();
	
    const char* vertexShaderCode = readFile(vertexShaderFilePath);
    const char* fragmentShaderCode = readFile(fragmentShaderFilePath);

    if(vertexShaderCode == nullptr || fragmentShaderCode == nullptr) {
        error = true;
        return 0;
    }


    if(!compileShader(vertexShaderID, vertexShaderCode, GL_VERTEX_SHADER)) {
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        error = true;
        return 0;
    }
    if(!compileShader(fragmentShaderID, fragmentShaderCode, GL_FRAGMENT_SHADER)) {
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        error = true;
        return 0;
    }


	if(!linkProgram(programID, vertexShaderID, fragmentShaderID)) {
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		error = true;
		return 0;
	}

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

GLFWwindow* initializeWindow(int windowWidth, int windowHeight, const char* windowTitle, int antialiasing, int openglMajorVersion, int openglMinorVersion, GLFWmonitor* windowMonitor, GLFWwindow* shareWindow) {

    glfwWindowHint(GLFW_SAMPLES, antialiasing);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinorVersion);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // to make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // no old OpenGL
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, windowMonitor, shareWindow);

    if(window == NULL) {
        infoLog = "Failed to open GLFW window.";
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);

    return window;
}

bool Renderer::init(int width, int height, const char* title, int antialiasing, int openglMajorVersion, int openglMinorVersion) {
    if(!glfwInit()) {
        infoLog = "Failed to initialize GLFW.";
        return false;
    }

    window = initializeWindow(width, height, title, antialiasing, openglMajorVersion, openglMinorVersion, NULL, NULL);
    if(window == NULL) {
        return false;
    }
    
    windowWidth = width;
    windowHeight = height;

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        infoLog = "Failed to initialize GLEW";
        return false;
    }

    return true;
}

bool Renderer::processShaders(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) {
	char* path = NULL;
	const char* vertPath = vertexShaderFilePath;
	const char* fragPath = fragmentShaderFilePath;
	std::ifstream vertFile(vertPath);
	if(!vertFile.is_open()) {
		path = strdup(__FILE__);
		path[strlen(path) - 14] = '\0';
		std::string tmp = std::string(path);
		tmp += vertPath == NULL ? "defaultVertexShader.vert" : vertPath;
		vertPath = strdup(tmp.c_str());
	}
	std::ifstream fragFile(fragPath);
	if(!fragFile.is_open()) {
		if(path == NULL) {
			path = strdup(__FILE__);
			path[strlen(path) - 14] = '\0';
		}
		std::string tmp = std::string(path);
		tmp += fragPath == NULL ? "defaultFragmentShader.frag" : fragPath;
		fragPath = strdup(tmp.c_str());
	}
    program = createProgram(vertPath, fragPath);
    return !error;
}


glm::vec3 cameraViewDirection;
glm::vec3 cameraRightVector;
glm::vec3 cameraUpVector;
glm::vec3 forwardMovementDirection;
glm::vec3 rightMovementDirection;

void Renderer::calculateCameraOrientationVectors() {
    cameraViewDirection = glm::vec3(
        cos(cameraRotation.y) * sin(cameraRotation.x),
        cos(cameraRotation.x),
        sin(cameraRotation.y) * sin(cameraRotation.x)
    );
    cameraRightVector = glm::vec3(
        -sin(cameraRotation.y),
        0,
        cos(cameraRotation.y)
    );
    glm::vec3 forwardMovementDirection = glm::normalize(glm::vec3(1, 0, 1) * cameraViewDirection);
    glm::vec3 rightMovementDirection = glm::normalize(glm::vec3(1, 0, 1) * cameraRightVector);
    cameraUpVector = glm::cross(cameraRightVector, cameraViewDirection);
}

glm::mat4 Renderer::calculateViewMatrix() {
    return glm::lookAt(cameraPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}
glm::mat4 Renderer::calculateProjectionMatrix() {
//    return glm::perspective(glm::radians(FOV), windowWidth / windowHeight, nearClipPlane, farClipPlane);
    return glm::perspective(glm::radians(FOV), (float) windowWidth / windowHeight, nearClipPlane, farClipPlane);
}

void Renderer::render() {
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    
    glUseProgram(program);
    
    calculateCameraOrientationVectors();
    glm::mat4 viewMatrix = calculateViewMatrix();
    glm::mat4 normalViewMatrix = NORMAL_MATRIX(viewMatrix);
    glm::mat4 viewProjectionMatrix = calculateProjectionMatrix() * viewMatrix;
    
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program, "normalViewMatrix"), 1, GL_FALSE, &normalViewMatrix[0][0]);
    
    glUniform3f(glGetUniformLocation(program, "cameraPosition"), cameraPosition.x, cameraPosition.y, cameraPosition.z);
    
    for(int j = 0;j < lights.size();j++) {
        auto generatePropertyName = [&](const char* property) -> char* {
            char* propertyName = (char*) malloc(sizeof(char) * (14 + strlen(property)));
            propertyName[0] = 'l';
            propertyName[1] = 'i';
            propertyName[2] = 'g';
            propertyName[3] = 'h';
            propertyName[4] = 't';
            propertyName[5] = 's';
            propertyName[6] = '[';
            propertyName[7] = round(j / 1000) + 48;
            propertyName[7] = round(j % 1000 / 100) + 48;
            propertyName[8] = round(j % 100 / 10) + 48;
            propertyName[9] = j % 10 + 48;
            propertyName[10] = ']';
            propertyName[11] = '.';
            memcpy(&propertyName[12], property, strlen(property) + 1);
            return propertyName;
        };
        glUniform1ui(glGetUniformLocation(program, generatePropertyName("type")), lights[j].type);
        glUniform3f(glGetUniformLocation(program, generatePropertyName("position")), lights[j].position.x, lights[j].position.y, lights[j].position.z);
        glUniform3f(glGetUniformLocation(program, generatePropertyName("direction")), lights[j].direction.x, lights[j].direction.y, lights[j].direction.z);
        glUniform3f(glGetUniformLocation(program, generatePropertyName("color")), lights[j].color.r, lights[j].color.g, lights[j].color.b);
        glUniform1f(glGetUniformLocation(program, generatePropertyName("intensity")), lights[j].intensity);
    }

    glUniform1ui(glGetUniformLocation(program, "lightCount"), (GLuint) lights.size());
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArray(vertexArrayObject);
    for(int i = 0;i < vertexCount.size();i++) {
        glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, &modelMatrices[i][0][0]);
        glm::mat4 normalModelMatrix = NORMAL_MATRIX(modelMatrices[i]);
        glUniformMatrix4fv(glGetUniformLocation(program, "normalModelMatrix"), 1, GL_FALSE, &normalModelMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(program, "MVPmatrix"), 1, GL_FALSE, &(viewProjectionMatrix * modelMatrices[i])[0][0]);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glUniform1i(glGetUniformLocation(program, "textureSampler"), 0);

        glUniform1f(glGetUniformLocation(program, "specularLightingIntensity"), specularLightingIntensity[i]);
        glUniform1f(glGetUniformLocation(program, "shininess"), shininess[i]);

        for(int j = 0;j < 3;j++) {
            glEnableVertexAttribArray(j);
            glBindBuffer(GL_ARRAY_BUFFER, buffers[4 * i + j]);
            glVertexAttribPointer(j, j == 1 ? 2 : 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[4 * i + 3]);
        glDrawElements(GL_TRIANGLES, vertexCount[i], GL_UNSIGNED_INT, (void *) 0);
        for(int j = 0;j < 3;j++) {
            glDisableVertexAttribArray(j);
        }
    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Renderer::createObject(GLuint vertexBuffer, GLuint UVbuffer, GLuint normalBuffer, GLuint elementBuffer, unsigned int vertexCount, GLuint texture, float specularLightingIntensity, float shininess) {
    buffers.push_back(vertexBuffer);
    buffers.push_back(UVbuffer);
    buffers.push_back(normalBuffer);
    buffers.push_back(elementBuffer);
    this->vertexCount.push_back(vertexCount);
    textures.push_back(texture);
    this->specularLightingIntensity.push_back(specularLightingIntensity);
    this->shininess.push_back(shininess);
}

std::string Renderer::getInfoLog() {
    return infoLog;
}

void Renderer::setVertexArrayObject(GLuint vertexArrayObject) {
    this->vertexArrayObject = vertexArrayObject;
}
