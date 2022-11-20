#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying vec2 vTexcoord;

void main()
{
    // Set fragment color from texture
    gl_FragColor = texture2D(texture, vTexcoord);
}
