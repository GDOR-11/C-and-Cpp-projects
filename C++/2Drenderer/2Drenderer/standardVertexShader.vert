const char* vertexShaderCode = R"(

#version 330

layout(location = 0) in vec2 vertexPosition;

uniform vec2 windowSize;

uniform vec2 objectPosition;
uniform mat2 transformMatrix;


void main() {
    gl_Position.xy = vec2(2, -2) * ((transformMatrix * vertexPosition + objectPosition) / windowSize - 0.5);
    gl_Position.zw = vec2(0, 1);
}

)";
