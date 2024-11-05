attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform float uvOffset;

varying vec2 outUv;
varying vec2 outUv2;
varying vec3 eyespacePosition;
varying vec3 eyespaceNormal;

void main(void)
{
	vec4 vertexPosition = vec4(position, 1.0);
	outUv = uv;
	outUv2 = uv + uvOffset;
	eyespacePosition = (modelMatrix * vertexPosition).xyz;
	eyespaceNormal = (modelMatrix * vec4(normal, 0.0)).xyz;
	gl_Position = modelViewProjectionMatrix * vertexPosition;
}