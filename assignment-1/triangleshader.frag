uniform sampler2D texture01;
varying vec3 outRgb;

void main(void){
    gl_FragColor = vec4(outRgb[0], outRgb[1], outRgb[2], 1.0);
}