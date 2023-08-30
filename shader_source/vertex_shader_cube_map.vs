attribute vec3 vCubePosition;
varying vec3 outCoord;

uniform mat4 view;
uniform mat4 projection;
void main() {
    vec4 pos = projection * view * vec4(vCubePosition, 1.0);
    outCoord = vCubePosition;
    gl_Position = pos.xyww;
}