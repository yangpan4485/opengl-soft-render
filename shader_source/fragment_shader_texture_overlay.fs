precision mediump float;
varying vec2 outCoord;
uniform sampler2D vTexture;
uniform sampler2D vOverlayTexture;
void main() {
    gl_FragColor = mix(texture2D(vTexture, outCoord), texture2D(vOverlayTexture, outCoord), 0.1);
}