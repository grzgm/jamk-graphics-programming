uniform sampler2D texture01;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform vec4 materialEmissive;
uniform float specularPower;

varying vec2 outUv;
varying vec3 eyespacePosition;
varying vec3 eyespaceNormal;

void main(void)
{
    vec3 normal = normalize(eyespaceNormal);
    vec3 surfaceToLight = normalize(lightPosition - eyespacePosition);
    float diffuseFactor = dot(normal, surfaceToLight);

    vec4 textureColor = texture2D(texture01, outUv);
    if (textureColor.a < 0.1) {
        discard;
    }

    vec4 diffuse = textureColor * materialDiffuse * diffuseFactor;

    vec4 finalColor;

    if (specularPower > 0.9)
    {
        vec3 surfaceToCamera = normalize(cameraPosition - eyespacePosition);
        float specularFactor = dot(surfaceToCamera, reflect(-surfaceToLight, normal));
        specularFactor = pow(max(0.0, specularFactor), specularPower);
        vec4 specular = materialSpecular * specularFactor;
        finalColor = materialAmbient + diffuse + materialEmissive + specular;
    }
    else
    {
        finalColor = materialAmbient + diffuse + materialEmissive;
    }

    gl_FragColor = vec4(finalColor.rgb, textureColor.a);
}
