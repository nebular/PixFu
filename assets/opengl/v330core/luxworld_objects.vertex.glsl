#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;
out vec3 FragPos;

out vec3 surfaceNormal;
out vec3 toCameraVector;

out vec3 directionalLightAmbient;
out vec3 directionalLightDiffuse;
out vec3 directionalLightSpecular;

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

// DIRECTIONAL LIGHT
struct DirLight {
    vec3 direction;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 invViewMatrix;

uniform DirLight dirLight;
uniform Material material;

void CalcDirLight(DirLight light) {

    vec3 lightDir = normalize(-light.direction);; // normalize(-light.direction);

	// diffuse shading
    float diff = max(dot(surfaceNormal, lightDir), 0.0);
    
	// specular shading
    vec3 reflectDir = reflect(-lightDir, surfaceNormal);
    float spec = material.illum==2 ? pow(max(dot(toCameraVector, reflectDir), 0.0), material.shininess) : 0;
    
	// combine results
    directionalLightAmbient  = light.ambient  * material.ambient;
    directionalLightDiffuse  = light.diffuse  * diff * material.diffuse;
    directionalLightSpecular = light.specular * spec * material.specular;
}


void main()
{

	vec4 worldPosition = transformationMatrix * vec4(aPos,1.0);

	// pre-normalize for fragment
	surfaceNormal = normalize((transformationMatrix * vec4(normal,0.0)).xyz);
	toCameraVector = normalize((invViewMatrix * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz);
	TexCoords = vec2(aTexCoord.x,1-aTexCoord.y);

	// calculate directional light
	CalcDirLight(dirLight);

	gl_Position = projectionMatrix * viewMatrix * worldPosition;
	FragPos = worldPosition.xyz;

}

