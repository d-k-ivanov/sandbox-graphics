uniform vec3 uMaterialColor;

uniform vec3 uDirLightPos;
uniform vec3 uDirLightColor;

uniform float uKd;
uniform float uScale;
uniform float uGainAlpha;

varying vec3 vNormal;
varying vec3 vViewPosition;
varying vec3 vModelPosition;

// from Schlick, "Fast alternatives to Perlin's bias and gain functions"
float computeBias(float t, float alpha)
{
    float exponent = -log(alpha) / log(2.0);
    return pow(t, exponent);
}

void main()
{
    // compute direction to light
    vec4 lDirection = viewMatrix * vec4(uDirLightPos, 0.0);
    vec3 lVector = normalize(lDirection.xyz);

    // diffuse: N * L. Normal must be normalized, since it's interpolated.
    vec3 normal = normalize(vNormal);

    float diffuse = max(dot(normal, lVector), 0.0);
    float attenuation = (0.5 + 0.5 * sin(uScale * sqrt(vModelPosition.x * vModelPosition.x + vModelPosition.z * vModelPosition.z)));

    // general gain formula
    diffuse *= (attenuation < 0.5) ? 0.5 * computeBias(2.0 * attenuation, uGainAlpha) : 1.0 - 0.5 * computeBias(2.0 - 2.0 * attenuation, uGainAlpha);

    // Student: instead output diffuse, uGainAlpha, attenuation to RGB
    // gl_FragColor = vec4(uKd * uMaterialColor * uDirLightColor * diffuse, 1.0);
    // gl_FragColor = vec4( diffuse, uGainAlpha, attenuation, 1.0 );
    gl_FragColor.r = diffuse;
    gl_FragColor.g = uGainAlpha;
    gl_FragColor.b = attenuation;
    gl_FragColor.a = 1.0;
}
