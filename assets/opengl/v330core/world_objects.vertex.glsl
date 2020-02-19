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

uniform vec3 lightPosition;

void main()
{
	
	vec4 worldPosition = transformationMatrix * vec4(aPos,1.0);
//	worldPosition = vec4(worldPosition.x, -worldPosition.z, worldPosition.y, 1.0);
//	vec3 surfaceNormal = (transformationMatrix * vec4(normal,0.0)).xyz;

	vec3 anormal = (transformationMatrix * vec4(normal,0.0)).xyz;
    surfaceNormal = 0.5 * ( anormal + vec3( 1 ) );

	toLightVector = lightPosition - worldPosition.xyz;
	toCameraVector = (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz;

	TexCoords = aTexCoord;
	
	gl_Position = projectionMatrix * viewMatrix * worldPosition;
//	gl_Position = vec4(aPos,1.0); // worldPosition;
}
