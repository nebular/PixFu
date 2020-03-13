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

/////////////////////////////////////////////////////////

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

// POINT LIGHT
struct PointLight {

	vec3 position;
    
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	vec3 params; // constant, linear, quadratic
	int enabled;
};

struct SpotLight {

	vec3 position;
    vec3 direction;
	
    vec2 cutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	vec3 params; // constant, linear, quadratic
	int enabled;

};


uniform float iTime;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_POINT_LIGHTS];

uniform int  lightMode;
uniform vec4 tintMode;

uniform Material material;
uniform sampler2D materialTexture;

vec3 resultLight;

void CalcSpotLight(SpotLight light, vec3 fragpos, vec3 incolor) {

	if (light.enabled==0.) return;
	
	vec3 lightDir = normalize(light.position - FragPos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));
    
    if(theta > light.cutOff.x) {

		// remember that we're working with angles as cosines instead of degrees
		// so a '>' is used.

		float distance    = length(light.position - FragPos);
		float attenuation = 1.0 / (light.params.x + light.params.y * distance + light.params.z * (distance * distance));

		// ambient
		vec3 ambient  = (light.ambient * material.ambient);

		// diffuse
		float diff = max(dot(surfaceNormal, lightDir), 0.0);
		vec3 diffuse  = light.diffuse * diff * incolor * attenuation;

		// specular
		vec3 viewDir = normalize(toCameraVector - FragPos);
		vec3 reflectDir = reflect(-lightDir, surfaceNormal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = material.illum==2 ? light.specular * spec * material.specular * attenuation : vec3(0,0,0);

		resultLight += ambient + diffuse + specular;

	}
}

void CalcPointLight(PointLight light,  vec3 fragPos, vec3 incolor)
{
	if (light.enabled==0) return;
	
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
    vec3 ambient  = (light.ambient  		* material.ambient) * attenuation;
    vec3 diffuse  = (light.diffuse  * diff 	* incolor) * attenuation;
	vec3 specular = material.illum==2 ? light.specular * spec * material.specular * attenuation : vec3(0,0,0);
	resultLight += ambient + diffuse + specular;
	
}

vec3 applyLightModel2(vec4 fincolor) {
	
    // phase 1: Directional lighting
	resultLight = directionalLightAmbient
		+ directionalLightDiffuse*fincolor.xyz
		+ directionalLightSpecular;
	
	if (lightMode == 0) return resultLight;
	
	// phase 2: Point lights
	for(int i = 0; i < NR_POINT_LIGHTS; i++) {
		CalcPointLight(pointLights[i], FragPos, fincolor.xyz);
		CalcSpotLight(spotLights[i], FragPos, fincolor.xyz);
	}
		
	return resultLight;
}

void main() {

	vec2 coords = TexCoords;
	vec4 reg = material.animRegion;

	// syntax is strange because I read somewhere that IF is very slow
	// compared to ternary operator
	
	// check point is into texture animation region
	bool apply= material.animConfig.z != 0
		&& coords.x >= reg.x && coords.y >= reg.y
		&& coords.x <= reg.z && coords.y <= reg.w;

	// if inside, apply animation offset
	coords += apply
		? material.animConfig.xy * iTime * material.animConfig.z
		: vec2(0., 0.);

	// keep within bounds
	coords += vec2(
				   (apply && (coords.x > reg.z)) ? reg.x - reg.z : 0.,
				   (apply && (coords.y > reg.y)) ? reg.y - reg.w : 0.
				   );

	vec4 fincolor = material.hasTexture == 1
		? texture(materialTexture, coords)
		: vec4(1,1,1,1);
	
	// tint
	if (tintMode.w==1.) fincolor*=tintMode;

	/////////////////////////////////////////////////////////////// lighting
	color = vec4(applyLightModel2(fincolor), fincolor.w);
}
