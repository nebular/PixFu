#version 330 core
#define NR_POINT_LIGHTS 4

in vec2 TexCoords;					// texture coordinates
in vec3 FragPos;					// Fragment Position (calc in vertex)
in vec3 surfaceNormal;				// Current Surface Normal
in vec3 toCameraVector;				// View Direction
in vec3 directionalLightAmbient;	// Directional Light: Ambient Component
in vec3 directionalLightDiffuse;	// Directional Light: Diffuse Component
in vec3 directionalLightSpecular; 	// Directional Light: Specular Component.

out vec4 color;						// Out color

// MATERIAL
struct Material {
    vec3 ambient;		// Material Ambient Color
    vec3 diffuse;		// Diffuse color
    vec3 specular;		// Specular color
    float shininess;	// Shininess (Ns)
	int hasTexture;		// Whether
	int illum;			// Illumination Code
	vec4 animRegion;	// [u0, v0, u1, v1]
	vec3 animConfig;	// [du, dv, on]
};

// DIRECTIONAL LIGHT
struct DirLight {
	vec3 direction; 	// light direction
	vec3 ambient;		// ambient color
    vec3 diffuse;		// diffuse color
    vec3 specular;		// specular color
};

// POINT LIGHT
struct PointLight {

	vec3 position;		// light position
    
	vec3 ambient;		// ambient color
    vec3 diffuse;		// diffuse color
    vec3 specular;		// specular color
	
	vec3 params; 		// constant, linear, quadratic
	int enabled;
};

struct SpotLight {

	vec3 position;		// light position
    vec3 direction;		// light direction
	
    vec2 cutOff;		// cutoff and outer cutoff
    vec3 ambient;		// ambient color
    vec3 diffuse;		// diffuse color
    vec3 specular;		// specular color
	
	vec3 params; 		// constant, linear, quadratic
	int enabled;		// whether

};

uniform float iTime;
uniform int   lightMode; 	// disable point lights (day mode)

uniform DirLight 	dirLight;
uniform PointLight 	pointLights[NR_POINT_LIGHTS];
uniform SpotLight 	spotLights[NR_POINT_LIGHTS];
uniform Material 	material;

uniform sampler2D materialTexture;
uniform sampler2D dirtyTexture;

vec3 zeros = vec3(0,0,0);

vec3 CalcDirLight(DirLight light, vec4 fincolor) {

    vec3 lightDir = normalize(-light.direction);; // normalize(-light.direction);

	// diffuse shading
    float diff = max(dot(surfaceNormal, lightDir), 0.0);
    
	// specular shading
    vec3 reflectDir = reflect(-lightDir, surfaceNormal);
    float spec = material.illum==2 ? pow(max(dot(toCameraVector, reflectDir), 0.0), material.shininess) : 0;
    
	// combine results
    return
			  light.ambient  * material.ambient
			+ light.diffuse  * diff * material.diffuse * fincolor.xyz
			+ light.specular * spec * material.specular;

}

vec3 calcSpotlightInBounds(SpotLight light, vec3 fragPos, vec3 incolor, vec3 lightDir, float intensity) {

	
	// remember that we're working with angles as cosines instead of degrees
	// so a '>' is used.

	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.params.x + light.params.y * distance + light.params.z * (distance * distance));

	// ambient
	vec3 ambient  = (light.ambient * material.ambient);

	// diffuse
	float diff = max(dot(surfaceNormal, lightDir), 0.0);
	vec3 diffuse  = light.diffuse  * diff * incolor * attenuation * intensity;

	// specular
	vec3 viewDir = normalize(toCameraVector - fragPos);
	vec3 reflectDir = reflect(-lightDir, surfaceNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = material.illum == 2 ? light.specular * spec * material.specular * attenuation * intensity : zeros;

	return ambient + diffuse + specular;

}

vec3 CalcSpotLight(SpotLight light, vec3 fragPos, vec3 incolor) {
	
	vec3 lightDir = normalize(light.position - fragPos);

	// check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff.x - light.cutOff.y;
	float intensity = clamp((theta - light.cutOff.y) / epsilon, 0.0, 1.0);

	// if light is within bounds, do the calculations
	// (confirm) shaders run ternary operations very efficiently vs a if

	return theta > light.cutOff.y
		? calcSpotlightInBounds(light, fragPos, incolor, lightDir, intensity)
		: zeros;
	
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
    float attenuation = 1.0 / (light.params.x + light.params.y * distance +
  			     light.params.z * (distance * distance));
	
	// combine results
    return
		  (light.ambient  			* material.ambient) * attenuation
		+ (light.diffuse  * diff 	* incolor) * attenuation
		+ (material.illum == 2 ? light.specular * spec * material.specular * attenuation : zeros);
}

vec3 applyLightSpots(vec4 fincolor) {

	vec3 resultLight = zeros;
	
	// phase 2: Point lights
	for(int i = 0; i < NR_POINT_LIGHTS; i++) {
		resultLight +=
			(pointLights[i].enabled == 1 ? CalcPointLight(pointLights[i], FragPos, fincolor.xyz) : zeros)
		  + (spotLights[i].enabled == 1 ? CalcSpotLight(spotLights[i], FragPos, fincolor.xyz) : zeros);
	}
	return resultLight;

}

void main() {

	// sample the sprite pixel
	vec4 dirtcolor = texture(dirtyTexture, TexCoords);
	vec4 fincolor = dirtcolor.a != 0
				? dirtcolor
				: material.hasTexture == 1
					? texture(materialTexture, TexCoords)
					: vec4(1,1,1,1);

	color = vec4(
				 CalcDirLight(dirLight, fincolor) + (lightMode != 0 ? applyLightSpots(fincolor) : zeros),
				 fincolor.w
			);


//	color = vec4(
//				 applyLightSpots(fincolor),
//				 fincolor.w
//			);
//	color = vec4(CalcDirLight(dirLight, fincolor), 1.);
	
}
