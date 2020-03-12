#version 330 core
struct Material {
    vec3 ambient;				// used
    vec3 diffuse;				// used
    vec3 specular;				// used
    float shininess;			// used
	int hasTexture;
	int illum;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	float ka;
};

in vec2 TexCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

out vec4 color;

uniform Light light;
uniform Material material;

uniform sampler2D materialTexture;
uniform sampler2D dirtyTexture;

uniform float iTime;

void main() {

	vec2 coords = TexCoords;
	
	vec4 fincolor=vec4(1,1,1,1);

	// sample the sprite pixel
	if (material.hasTexture==1)
		fincolor = texture(materialTexture, coords);

	// ambient, ka = a,bient weight
	vec3 ambient = light.ka * light.ka * light.ambient * material.ambient;

	// diffuse
	float diff = max(dot(surfaceNormal, toLightVector), 0.2);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	vec3 specular = vec3(0.,0.,0.);

	if (material.illum == 2) {
		// has specular
		vec3 reflectDir = reflect(-toLightVector, surfaceNormal);
		float spec = pow(max(dot(toCameraVector, reflectDir), 0.0), material.shininess);
		specular = light.specular * (spec * material.specular);
	}

	vec3 result = ambient + diffuse * fincolor.xyz + specular;
//	color = fincolor;
	color = vec4(result, 1.0);
}
