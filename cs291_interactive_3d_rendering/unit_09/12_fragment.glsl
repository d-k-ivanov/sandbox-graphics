uniform vec3 uMaterialColor;
uniform vec3 uSpecularColor;

uniform vec3 uDirLightPos;
uniform vec3 uDirLightColor;

uniform vec3 uAmbientLightColor;

uniform float uKd;
uniform float uKs;
uniform float shininess;

uniform float uDropoff;

varying vec3 vNormal;
varying vec3 vViewPosition;

void main()
{
    // ambient
    gl_FragColor = vec4(uAmbientLightColor, 1.0);

    // compute direction to light
    vec4 lDirection = viewMatrix * vec4(uDirLightPos, 0.0);
    vec3 lVector = normalize(lDirection.xyz);

    vec3 normal = normalize(vNormal);

    // diffuse: N * L. Normal must be normalized, since it's interpolated.
    float diffuse = max(dot(normal, lVector), 0.0);

    // This can give a hard termination to the highlight, but it's better than some weird sparkle.
    if(diffuse <= 0.0)
    {
        return;
    }

    gl_FragColor.rgb += uKd * uMaterialColor * uDirLightColor * diffuse;

    // specular: N * H to a power. H is light vector + view vector
    vec3 viewPosition = normalize(vViewPosition);
    vec3 pointHalfVector = normalize(lVector + viewPosition);
    float pointDotNormalHalf = max(dot(normal, pointHalfVector), 0.0);
    float specular = pow(pointDotNormalHalf, shininess);
    specular *= diffuse * (2.0 + shininess) / 8.0;

    // Student: add test here, when specular < uDropoff, set to 0.0, else to 1.0
    specular = (specular < uDropoff) ? 0.0 : 1.0;

    gl_FragColor.rgb += uDirLightColor * uSpecularColor * uKs * specular;
}
