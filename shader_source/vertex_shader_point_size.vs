attribute vec3 vPosition;
attribute vec3 vColor;
uniform float vPointSize;
varying vec3 outColor;
void main() {
    gl_Position = vec4(vPosition, 1.0);
    outColor = vColor;
    gl_PointSize = vPointSize;
}