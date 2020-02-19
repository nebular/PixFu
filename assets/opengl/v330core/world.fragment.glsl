#version 330 core

in vec2 TexCoords;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

out vec4 color;

uniform sampler2D modelTexture;
uniform vec3 lightColour;
uniform float shineDamper;
uniform float reflectivity;

void main()
{
	// coords in texture
	vec2 coords = TexCoords;
	// sample the sprite pixel
	vec4 fincolor = texture(modelTexture, coords);

	
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	
	float nDotl = dot(unitNormal,unitLightVector);
	float brightness = max(nDotl,0.2);
	vec3 diffuse = brightness * lightColour;
	
	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection,unitNormal);
	
	float specularFactor = dot(reflectedLightDirection , unitVectorToCamera);
	specularFactor = max(specularFactor,0.0);
	float dampedFactor = pow(specularFactor,shineDamper);
	vec3 finalSpecular = dampedFactor * reflectivity * lightColour;
	
	color =  vec4(diffuse,1.0) * fincolor + vec4(finalSpecular,1.0);

}
