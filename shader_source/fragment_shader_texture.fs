precision mediump float;
varying vec2 outCoord;
uniform sampler2D vTexture;
void main() {
    gl_FragColor = texture2D(vTexture, outCoord);
}