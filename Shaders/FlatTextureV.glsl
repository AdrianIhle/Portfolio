#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 uMVPMatrix;

attribute vec4 aVertexPosition;
attribute vec3 aNormal;
attribute vec2 aTexcoord;

varying vec2 vTexcoord;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = uMVPMatrix * aVertexPosition;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    vTexcoord = aTexcoord;

    //a hack to get shaderlinker to give aNormal a Location.
    //does nothing visual
    //should be changed...
    vec3 tempNormal = aNormal;
}
