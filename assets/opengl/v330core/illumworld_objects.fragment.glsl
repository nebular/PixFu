#version 330 core

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

uniform float iTime;
uniform Light light;
uniform Material material;

uniform sampler2D materialTexture;

uniform vec4 tintMode;

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
	
	/////////////////////////////////////////////////////////////// lighting
	
	// ambient
	vec3 ambient = light.ka * light.ambient * material.ambient;

	// diffuse
	float brightness = max(dot(surfaceNormal, toLightVector), 0.0);
	
	vec3 diffuse = light.diffuse * brightness * material.diffuse;
	vec3 specular = vec3(0.,0.,0.);

	if (material.illum == 2) {
		// has specular
		vec3 reflectDir = reflect(-toLightVector, surfaceNormal);
		float reflectivity = pow(max(dot(toCameraVector, reflectDir), 0.0), material.shininess);
		specular = light.specular * (reflectivity * material.specular);
	}
	
	// tint
	if (tintMode.w==1.) fincolor*=tintMode;

	vec3 result = ambient + diffuse * fincolor.xyz + specular;

//	color = fincolor;
	color = vec4(result, 1.0);

}
