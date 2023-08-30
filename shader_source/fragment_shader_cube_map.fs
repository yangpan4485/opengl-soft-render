precision mediump float;
varying vec3 outCoord;

uniform samplerCube skybox;
void main() {
    gl_FragColor = textureCube(skybox, outCoord);
}