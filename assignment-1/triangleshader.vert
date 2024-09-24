attribute vec3 position;
attribute vec3 rgb;

uniform mat4 mvpMatrix;

varying vec3 outRgb;

void main(void) {
    outRgb = rgb;
    gl_Position = mvpMatrix * vec4(position, 1.0);
}