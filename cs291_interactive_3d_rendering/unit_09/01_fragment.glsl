uniform vec3 uMaterialColor;
uniform vec3 uSpecularColor;

uniform vec3 uDirLightPos;
uniform vec3 uDirLightColor;

uniform vec3 uAmbientLightColor;

uniform float uKd;
uniform float uKs;
uniform float shininess;

uniform float uFresnelScale;

// KISS - three.js doesn't support bool for GLSL, AFAIK
uniform float uUseFresnel;

varying vec3 vNormal;
varying vec3 vViewPosition;

// for Blinn-Phong you want to pass in the H vector, the
// perfect mirror microfacet direction. Otherwise pass in N.
// See http://en.wikipedia.org/wiki/Schlick%27s_approximation for formula
// R0 is ((1-n2)/(1+n2))^2, e.g. for skin (from GPU Gems 3):
// Index of refraction of 1.4 gives ((1-1.4)/(1+1.4))^2 = 0.028
float fresnelReflectance(vec3 L, vec3 H, float R0)
{
    float base = 1.0 - dot(L, H);
    float exponential = pow(base, 5.0);
    return R0 + (1.0 - R0) * exponential;
}

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

    gl_FragColor.rgb += uKd * uMaterialColor * uDirLightColor * diffuse;

    // This can give a hard termination to the highlight, but it's better than some weird sparkle.
    if(diffuse > 0.0)
    {
        // specular: N * H to a power. H is light vector + view vector
        vec3 viewPosition = normalize(vViewPosition);
        vec3 pointHalfVector = normalize(lVector + viewPosition);
        float pointDotNormalHalf = max(dot(normal, pointHalfVector), 0.0);
        float specular = pow(pointDotNormalHalf, shininess);
        specular *= (2.0 + shininess) / 8.0;

        vec3 sc = uDirLightColor * uSpecularColor * uKs * specular;
        if(uUseFresnel != 0.0)
        {
            // Since Fresnel dims the specular considerably except at a shallow angle,
            // adjust by a fudge factor. We're not dealing with an energy-conserving
            // illumination model here.
            specular *= uFresnelScale * fresnelReflectance(lVector, pointHalfVector, 0.028);
        }
        gl_FragColor.rgb += uKd * uMaterialColor * uDirLightColor * diffuse;
        gl_FragColor.rgb += diffuse * uDirLightColor * uSpecularColor * uKs * specular;
    }
}
