uniform sampler2D texture01;

varying vec2 outUv;
varying vec4 outColor;

void main(void)
{
    gl_FragColor = outColor * texture2D(texture01, outUv);
}

