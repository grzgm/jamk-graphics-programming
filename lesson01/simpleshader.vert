attribute vec3 position;
attribute vec3 rgb;
// attribute vec2 uv;

uniform mat4 mvpMatrix;

varying vec3 outRgb;
// varying vec2 outUv;

void main(void) {
    outRgb = rgb;
    // outUv = uv;
    gl_Position = mvpMatrix * vec4(position, 1.0);
}