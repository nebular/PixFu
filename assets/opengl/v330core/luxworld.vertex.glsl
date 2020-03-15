#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 surfaceNormal;
out vec3 toCameraVector;

// MATERIAL
struct Material {
    vec3 ambient;				// used
    vec3 diffuse;				// used
    vec3 specular;				// used
    float shininess;			// used
	int hasTexture;
	int illum;
	vec4 animRegion;			// [x0, y0, x1, y1]
	vec3 animConfig;			// [dx, dy, on]
};

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 invViewMatrix;

uniform Material material;

void main()
{

	vec4 worldPosition = transformationMatrix * vec4(aPos,1.0);

	// pre-normalize for fragment
	surfaceNormal = normalize((transformationMatrix * vec4(normal,0.0)).xyz);

	toCameraVector = normalize((invViewMatrix * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz);
	TexCoords = vec2(aTexCoord.x,1-aTexCoord.y);

	gl_Position = projectionMatrix * viewMatrix * worldPosition;
	FragPos = worldPosition.xyz;
}
