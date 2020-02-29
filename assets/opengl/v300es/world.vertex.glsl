attribute vec3 aPos;
attribute vec3 normal;
attribute vec2 aTexCoord;

varying vec2 TexCoords;

varying vec3 surfaceNormal;
varying vec3 toLightVector;
varying vec3 toCameraVector;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 invViewMatrix;

uniform vec3 lightPosition;

void main()
{

	vec4 worldPosition = transformationMatrix * vec4(aPos,1.0);

	surfaceNormal = (transformationMatrix * vec4(normal,0.0)).xyz;
	toLightVector = lightPosition - worldPosition.xyz;
	toCameraVector = (invViewMatrix * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz;

	TexCoords = vec2(aTexCoord.x,1.-aTexCoord.y);
//	TexCoords = vec2(aTexCoord.x,aTexCoord.y);

	gl_Position = projectionMatrix * viewMatrix * worldPosition;

}
