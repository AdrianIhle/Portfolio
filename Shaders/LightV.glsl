#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec4 aVertexPosition;
attribute vec3 aNormal;
attribute vec2 aTexcoord;

varying vec2 vTexcoord;
varying vec3 vNormalEye;
varying vec3 vPositionEye3;

uniform mat4 mvMatrix;
uniform mat4 pMatrix;
uniform mat3 nMatrix;

uniform vec3 lightPosition;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;


void main()
{
    // Get vertex position in eye coordinates and send to the fragment shader
    vec4 vertexPositionEye4 = mvMatrix * aVertexPosition;
    vPositionEye3 = vertexPositionEye4.xyz / vertexPositionEye4.w;
    // Transform the normal to eye coordinates and send to fragment shader
    vNormalEye = normalize(nMatrix * aNormal);
    // Transform the geometry
    gl_Position = pMatrix * mvMatrix * aVertexPosition;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    vTexcoord = aTexcoord;
}
