#version 330 core
#define NR_POINT_LIGHTS 4

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
	float ka;
};


// POINT LIGHT
struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float ka;
	int enabled;
};

struct SpotLight {

	vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
	
	float ka;
	int enabled;

};

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 invViewMatrix;

uniform DirLight dirLight;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_POINT_LIGHTS];


void CalcDirLight(DirLight light) {

    vec3 lightDir = normalize(-light.direction);; // normalize(-light.direction);

	// diffuse shading
    float diff = max(dot(surfaceNormal, lightDir), 0.0);
    
	// specular shading
    vec3 reflectDir = reflect(-lightDir, surfaceNormal);
    float spec = material.illum==2 ? pow(max(dot(toCameraVector, reflectDir), 0.0), material.shininess) : 0;
    
	// combine results
    directionalLightAmbient  = light.ka * light.ambient  * material.ambient;
    directionalLightDiffuse  = light.diffuse  * diff * material.diffuse;
    directionalLightSpecular = light.specular * spec * material.specular;

	directionalLightAmbient  = vec3(0,0,0);
	directionalLightDiffuse  = vec3(0,0,0);
	directionalLightSpecular = vec3(0,0,0);
}

void CalcSpotLight(SpotLight light, vec3 fragpos) {

	if (light.enabled==0) return;
	
	vec3 lightDir = normalize(light.position - fragpos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));
    
    if(theta > light.cutOff) {

		// remember that we're working with angles as cosines instead of degrees
		// so a '>' is used.

		float distance    = length(light.position - fragpos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		// ambient
		vec3 ambient  = (light.ambient * material.ambient);

		// diffuse
		float diff = max(dot(surfaceNormal, lightDir), 0.0);
		vec3 diffuse  = (light.diffuse  * diff) * attenuation;

		// specular
		vec3 viewDir = normalize(toCameraVector - fragpos);
		vec3 reflectDir = reflect(-lightDir, surfaceNormal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = material.illum==2 ? light.specular * spec * material.specular * attenuation : vec3(0,0,0);

		directionalLightAmbient  += light.ka * ambient;
		directionalLightDiffuse  += diffuse;
		directionalLightSpecular += specular;
	}
}

void CalcPointLight(PointLight light,  vec3 fragPos)
{
	if (light.enabled!=1) return;
	
    vec3 lightDir = normalize(light.position - fragPos);
    
	// diffuse shading
    float diff = max(dot(surfaceNormal, lightDir), 0.0);
    
	// specular shading
    vec3 reflectDir = reflect(-lightDir, surfaceNormal);
    float spec = pow(max(dot(toCameraVector, reflectDir), 0.0), material.shininess);
    
	// attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
  			     light.quadratic * (distance * distance));
	
	// combine results
    vec3 ambient  = (light.ambient  		* material.ambient) * attenuation;
    vec3 diffuse  = (light.diffuse  * diff) * attenuation;
	vec3 specular = material.illum==2 ? light.specular * spec * material.specular * attenuation : vec3(0,0,0);

	directionalLightAmbient += light.ka * ambient;
	directionalLightDiffuse += diffuse;
	directionalLightSpecular += specular;

}


void main()
{

	vec4 worldPosition = transformationMatrix * vec4(aPos,1.0);

	// pre-normalize for fragment
	surfaceNormal = normalize((transformationMatrix * vec4(normal,0.0)).xyz);

	toCameraVector = normalize((invViewMatrix * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz);
	TexCoords = vec2(aTexCoord.x, 1-aTexCoord.y);

	CalcDirLight(dirLight);

	// phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
		CalcPointLight(pointLights[i], worldPosition.xyz);
		CalcSpotLight(spotLights[i], worldPosition.xyz);
	}
	
	gl_Position = projectionMatrix * viewMatrix * worldPosition;
	FragPos = worldPosition.xyz;
}
