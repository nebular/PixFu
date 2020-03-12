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
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
	float ka;
	int enabled;
};

uniform float iTime;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

uniform sampler2D materialTexture;

uniform vec4 tintMode;

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

vec4 applyLightModel2(vec4 fincolor) {

	// phase 1: Directional lighting
	vec3 result = directionalLightAmbient
		+ directionalLightDiffuse*fincolor.xyz
		+ directionalLightSpecular;

	// phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
		if (pointLights[i].enabled == 1)
			result += CalcPointLight(pointLights[i], FragPos, fincolor.xyz);

	// 		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
    return vec4(result, fincolor.w);
	
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
	color = applyLightModel2(fincolor);
}
