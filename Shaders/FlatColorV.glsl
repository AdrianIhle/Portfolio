#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

attribute vec4 aVertexPosition;
attribute vec3 aNormal;
attribute vec2 aTexcoord;

varying vec4 vColor;

uniform mat4 uMVPMatrix;

void main() {
    //using normal as color:
    //normals is sometimes negative witch give a black color
    //so we take abs value of it.
    vec3 absNormal = abs(aNormal);
    vColor = vec4(absNormal, 1.0);
    gl_Position = uMVPMatrix * aVertexPosition;

    //Must use aTexcoord or it will not get a Location in linker.
    //So this sentence is just to use it, it does nothing visual.
    absNormal.x = aTexcoord.x;
}
