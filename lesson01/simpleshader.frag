uniform sampler2D texture01;
varying vec3 outRgb;
// varying vec2 outUv;

void main(void){
    // gl_FragColor = vec4(0.0, 0.2, 0.4, 1.0);
    // gl_FragColor = vec4(outUv[0], outUv[1], 0.4, 1.0);
    // gl_FragColor = texture2D(texture01, outUv);
    gl_FragColor = vec4(outRgb[0], outRgb[1], outRgb[2], 1.0);
}