#version 330

#define MAX_LIGHTS 16
#define AMBIENT_LIGHT 0
#define DIRECTIONAL_LIGHT 1
#define POINT_LIGHT 2

struct LightSource {
    uint type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
};

uniform sampler2D textureSampler;
uniform vec3 cameraPosition;
uniform LightSource lights[MAX_LIGHTS];
uniform uint lightCount;
uniform float specularLightingIntensity;
uniform float shininess;


const float k = 4;


in vec3 positionCameraSpace;
in vec3 positionWorldSpace;
in vec3 positionObjectSpace;
in vec3 normalCameraSpace;
in vec3 normalWorldSpace;
in vec3 normalObjectSpace;
in vec2 UV;
in vec4 glposition;

out vec4 fragmentColor;

vec3 aces_tonemap(vec3 color){
	mat3 m1 = mat3(
		0.59719, 0.07600, 0.02840,
		0.35458, 0.90834, 0.13383,
		0.04823, 0.01566, 0.83777
	);
	mat3 m2 = mat3(
		1.60475, -0.10208, -0.00327,
		-0.53108,  1.10813, -0.07276,
		-0.07367, -0.00605,  1.07602
	);
	vec3 v = m1 * color;
	vec3 a = v * (v + 0.0245786) - 0.000090537;
	vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
	return clamp(m2 * (a / b), 0.0, 1.0);
}

float calculateSpecularLighting(vec3 reflection) {
//	return pow(clamp(dot(normalize(cameraPosition - positionWorldSpace), reflection), 0, 1), shininess + tan(acos(dot(normalize(cameraPosition - positionWorldSpace), normalWorldSpace)) / 2));
	return pow(clamp(dot(normalize(cameraPosition - positionWorldSpace), reflection), 0, 1), shininess);
}
float calculateDiffuseLighting(vec3 rayDirection) {
    return clamp(dot(normalWorldSpace, -rayDirection), 0, 1);
}
vec3 calculateLighting(float intensity, vec3 color, float specularLighting, float diffuseLighting) {
    return intensity * color * (specularLightingIntensity * specularLighting + (1 - specularLightingIntensity) * diffuseLighting);
}

void main() {
    fragmentColor = vec4(0, 0, 0, 1);
	
//	only use this for lighting models wich don't work for nonsensical normal vectors
//	if(dot(cameraPosition - positionWorldSpace, normalWorldSpace) <= 0) {
//		fragmentColor = vec4(0, 0, 0, 0);
//		return;
//	}
	
    for(uint i = 0u;i < lightCount;i++) {
        LightSource light = lights[i];

        float lightDistance = distance(light.position, positionWorldSpace);
        vec3 rayDirection;
        vec3 reflection;
        float specularLighting;
        float diffuseLighting;


        switch(light.type) {
            case AMBIENT_LIGHT:
                fragmentColor.xyz += light.color * light.intensity;
                break;
            case DIRECTIONAL_LIGHT:
                rayDirection = normalize(light.direction);
				if(dot(normalWorldSpace, rayDirection) > 0) break;
                reflection = reflect(rayDirection, normalWorldSpace);
                specularLighting = calculateSpecularLighting(reflection);
                diffuseLighting = calculateDiffuseLighting(rayDirection);
                fragmentColor.xyz += calculateLighting(light.intensity, light.color, specularLighting, diffuseLighting);
                break;
            case POINT_LIGHT:
                rayDirection = normalize(positionWorldSpace - light.position);
                if(dot(normalWorldSpace, rayDirection) > 0) break;
                reflection = reflect(rayDirection, normalWorldSpace);

                specularLighting = calculateSpecularLighting(reflection);
                diffuseLighting = calculateDiffuseLighting(rayDirection);
                fragmentColor.xyz += calculateLighting(light.intensity, light.color, specularLighting, diffuseLighting) / (lightDistance * lightDistance);
                break;
        }
    }
	fragmentColor.xyz = aces_tonemap(fragmentColor.xyz);
    fragmentColor *= texture(textureSampler, UV);
//	fragmentColor = vec4(vec3(glposition.w / 10), 1);
}
