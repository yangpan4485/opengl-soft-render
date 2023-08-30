attribute vec3 vPosition;
attribute vec2 vCoord;
varying vec2 outCoord;
void main() {
    gl_Position = vec4(vPosition, 1.0);
    outCoord = vCoord;
}