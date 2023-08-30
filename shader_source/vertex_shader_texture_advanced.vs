attribute vec3 vPosition;
attribute vec2 vCoord;
varying vec2 outCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(vPosition, 1.0);
    outCoord = vCoord;
}