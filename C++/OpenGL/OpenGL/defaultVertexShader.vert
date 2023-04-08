#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 normalModelMatrix;
uniform mat4 normalViewMatrix;
uniform mat4 MVPmatrix;

uniform vec3 cameraPosition;

out vec3 positionCameraSpace;
out vec3 positionWorldSpace;
out vec3 positionObjectSpace;
out vec3 normalCameraSpace;
out vec3 normalWorldSpace;
out vec3 normalObjectSpace;
out vec2 UV;
out vec4 glposition;

void main() {
    gl_Position = MVPmatrix * vec4(vertexPosition, 1);
	glposition = gl_Position;
    
    positionCameraSpace = (viewMatrix * vec4(vertexPosition, 1)).xyz;
    positionWorldSpace = (modelMatrix * vec4(vertexPosition, 1)).xyz;
    positionObjectSpace = vertexPosition;
	
	// https://i.stack.imgur.com/bChxr.png
	normalCameraSpace = normalize((normalViewMatrix * vec4(vertexNormal, 0)).xyz);
	normalWorldSpace = normalize((normalModelMatrix * vec4(vertexNormal, 0)).xyz);
	normalObjectSpace = vertexNormal;
    
//    vec4 cameraVector = vec4(normalize(cameraPosition - positionWorldSpace), 0);
//    if(dot(cameraVector, vec4(normalWorldSpace, 0)) <= 0) {
//        normalWorldSpace = -normalize(cross(cameraVector.xyz, cross(cameraVector.xyz, normalWorldSpace)));
//        for(;dot(cameraVector, vec4(normalWorldSpace, 0)) <= 0;normalWorldSpace += cameraVector.xyz * 0.1);
//        normalObjectSpace = (vec4(normalWorldSpace, 0) * inverse(normalModelMatrix)).xyz;
//    }
//    normalCameraSpace = (normalViewMatrix * vec4(normalObjectSpace, 0)).xyz;

    UV = vertexUV;
}
