uniform vec3 uMaterialColor;

uniform vec3 uDirLightPos;
uniform vec3 uDirLightColor;

uniform float uKd;
uniform float uBorder;

varying vec3 vNormal;

void main()
{
    // compute direction to light
    vec4 lDirection = viewMatrix * vec4(uDirLightPos, 0.0);
    vec3 lVector = normalize(lDirection.xyz);

    // diffuse: N * L. Normal must be normalized, since it's interpolated.
    vec3 normal = normalize(vNormal);

    // Student: check the diffuse dot product against uBorder and adjust
    // this diffuse value accordingly.
    // float diffuse = max(dot(normal, lVector), 0.0);
    float diffuse;
    if(dot(normal, lVector) > uBorder)
    {
        diffuse = 1.0;
    }
    else
    {
        diffuse = 0.5;
    }

    gl_FragColor = vec4(uKd * uMaterialColor * uDirLightColor * diffuse, 1.0);
}
