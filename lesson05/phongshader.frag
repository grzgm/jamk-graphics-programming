uniform sampler2D texture01;

uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform vec4 materialEmissive;
uniform float specularPower;

uniform vec3 lightDirection;
uniform vec3 cameraPosition;

varying vec2 outUv;
varying vec3 eyespacePosition;
varying vec3 eyespaceNormal;

void main(void)
{
    gl_FragColor = texture2D(texture01, outUv);
}

