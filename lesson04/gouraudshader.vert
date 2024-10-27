attribute vec3 position;
attribute vec3 normal;
attribute vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightDirection;

varying vec2 outUv;

varying vec4 outColor;

void main(void)
{
	mat4 mv = viewMatrix * modelMatrix;
	vec3 modelViewNormal = vec3(mv * vec4(normal, 0.0));
	float diffuseFactor = dot(modelViewNormal, -lightDirection);

	outUv = uv;
	outColor = vec4(1.0, 1.0, 1.0, 1.0) * diffuseFactor;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}

