#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;

out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 invViewMatrix;

uniform vec3 lightPosition;

uniform float iTime;

void main()
{
	
	vec4 worldPosition = transformationMatrix * vec4(aPos,1.0);

	vec3 anormal = (transformationMatrix * vec4(normal,0.0)).xyz;
    surfaceNormal = normalize(0.5 * ( anormal + vec3( 1 ) ));

//	vec3 lp = vec3(lightPosition.x*sin(iTime), lightPosition.y, lightPosition.z*cos(iTime));
//	toLightVector = normalize(lp - worldPosition.xyz);

	toLightVector = normalize(lightPosition - worldPosition.xyz);
	toCameraVector = normalize((invViewMatrix * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz);

	TexCoords = vec2(aTexCoord.x, 1-aTexCoord.y);
	
	gl_Position = projectionMatrix * viewMatrix * worldPosition;

}
