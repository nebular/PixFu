#version 330 core
#define NR_POINT_LIGHTS 4

in vec2 TexCoords;
in vec3 FragPos;
in vec3 surfaceNormal;
in vec3 toCameraVector;
in vec3 directionalLightAmbient;
in vec3 directionalLightDiffuse;
in vec3 directionalLightSpecular;

out vec4 color;


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

uniform float iTime;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_POINT_LIGHTS];
uniform Material material;

uniform sampler2D materialTexture;
uniform sampler2D dirtyTexture;

vec3 CalcSpotLight(SpotLight light, vec3 fragpos, vec3 incolor) {

	vec3 lightDir = normalize(light.position - FragPos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));
    
    if(theta > light.cutOff) {

		// remember that we're working with angles as cosines instead of degrees
		// so a '>' is used.

		float distance    = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		// ambient
		vec3 ambient  = (light.ambient * material.ambient);

		// diffuse
		float diff = max(dot(surfaceNormal, lightDir), 0.0);
		vec3 diffuse  = (light.diffuse  * diff 	* incolor) * attenuation;

		// specular
		vec3 viewDir = normalize(toCameraVector - FragPos);
		vec3 reflectDir = reflect(-lightDir, surfaceNormal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = material.illum==2 ? light.specular * spec * material.specular * attenuation : vec3(0,0,0);

		return light.ka * ambient + diffuse + specular;

	} else {
		// else, use ambient light so scene isn't completely dark outside the spotlight.
		return vec3(0,0,0);
		// FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
	  }
}

vec3 CalcPointLight(PointLight light,  vec3 fragPos, vec3 incolor)
{
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
    vec3 diffuse  = (light.diffuse  * diff 	* incolor) * attenuation;
	vec3 specular = material.illum==2 ? light.specular * spec * material.specular * attenuation : vec3(0,0,0);
	return light.ka * ambient + diffuse + specular;
	
}

vec4 applyLightModel2(vec4  fincolor) {
	
    // phase 1: Directional lighting
	vec3 result = directionalLightAmbient
		+ directionalLightDiffuse*fincolor.xyz
		+ directionalLightSpecular;
	
    return vec4(result, fincolor.w);

}

void main() {

	vec2 coords = TexCoords;
	//	color = texture(materialTexture, coords);

	// sample the sprite pixel
	vec4 dirtcolor = texture(dirtyTexture, coords);
	vec4 fincolor = dirtcolor.a != 0
				? dirtcolor
				: material.hasTexture == 1
					? texture(materialTexture, coords)
					: vec4(1,1,1,1);

	color = applyLightModel2(fincolor);

}
