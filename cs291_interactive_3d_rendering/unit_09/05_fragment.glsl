uniform vec3 uMaterialColor;

uniform vec3 uDirLightPos;
uniform vec3 uDirLightColor;

uniform float uKd;
uniform float uScale;

varying vec3 vNormal;
varying vec3 vViewPosition;
varying vec3 vModelPosition;

float modulo(float x)
{
    return x - floor(x);
}

void main()
{
    // compute direction to light
    vec4 lDirection = viewMatrix * vec4(uDirLightPos, 0.0);
    vec3 lVector = normalize(lDirection.xyz);

    // diffuse: N * L. Normal must be normalized, since it's interpolated.
    vec3 normal = normalize(vNormal);

    // Student: use the vModelPosition as an input to a function and then multiply the diffuse contribution by this amount.
    float pattern;
    float diffuse;
    // pattern = sin(uScale * vModelPosition.x);
    // pattern = sin(uScale * vModelPosition.y);
    // pattern = sin(uScale * vModelPosition.z);
    pattern = sin(uScale * vModelPosition.x) * sin(uScale * vModelPosition.y) * sin(uScale * vModelPosition.z);
    // pattern = sin(uScale * vViewPosition.x) * sin(uScale * vViewPosition.y) * sin(uScale * vViewPosition.z);
    // pattern = modulo(uScale * vModelPosition.x);
    // pattern = modulo(uScale * vModelPosition.y);
    // pattern = modulo(uScale * vModelPosition.z);
    // pattern = modulo(uScale * vModelPosition.x) * modulo(uScale * vModelPosition.y) * modulo(uScale * vModelPosition.z);
    // pattern = modulo(uScale * vViewPosition.x) * modulo(uScale * vViewPosition.y) * modulo(uScale * vViewPosition.z);

    // diffuse = max(dot(normal, lVector), 0.0);
    // diffuse = pattern;
    diffuse = (0.5 + 0.5 * pattern);

    gl_FragColor = vec4(uKd * uMaterialColor * uDirLightColor * diffuse, 1.0);
}
