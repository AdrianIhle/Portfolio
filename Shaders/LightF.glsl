#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

varying vec2 vTexcoord;
varying vec3 vNormalEye;
varying vec3 vPositionEye3;

uniform vec3 lightPosition;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform sampler2D texture;

const float shininess = 32.0;

void main()
{
    // Calculate the vector (l) to the light source
    vec3 vectorToLightSource = normalize(lightPosition - vPositionEye3);
    // Calculate n dot l for diffuse lighting
    float diffuseLightWeighting = max(dot(vNormalEye, vectorToLightSource), 0.0);
    // Calculate the reflection vector (r) that is needed for specular light
    vec3 reflectionVector = normalize(reflect(-vectorToLightSource, vNormalEye));
    // Camera in eye space is in origin pointing along the negative z-axis.
    // Calculate viewVector (v) in eye coordinates as
    // (0.0, 0.0, 0.0) - vPositionEye3
    vec3 viewVectorEye = -normalize(vPositionEye3);

    float rdotv = max(dot(reflectionVector, viewVectorEye), 0.0);
    float specularLightWeighting = pow(rdotv, shininess);
    // Sum up all three reflection components
    vec3 lightWeighting = ambientColor +
            diffuseColor * diffuseLightWeighting +
            specularColor * specularLightWeighting;
    // Sample the texture
    vec4 texelColor = texture2D(texture, vTexcoord);
    // modulate texel color with lightweigthing and write as final color
    gl_FragColor = vec4(lightWeighting.rgb * texelColor.rgb, texelColor.a);

    // Set fragment color from texture - no lights if you uncomment this:
    //gl_FragColor = texelColor;
}

