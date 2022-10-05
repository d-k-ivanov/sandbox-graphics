varying vec3 vNormal;
varying vec3 vViewPosition;

uniform float uSphereRadius2; // squared

void main()
{
    // Student: modify position. You'll need to copy it to a temporary vector and modify the temporary.
    // Don't forget to use this temporary in the rest of the shader.
    vec3 pos = position;
    pos.z = sqrt(uSphereRadius2 - pos.x * pos.x - pos.y * pos.y) - sqrt(uSphereRadius2);

    gl_Position = projectionMatrix * modelViewMatrix * vec4(pos, 1.0);
    vNormal = normalize(normalMatrix * normal);
    vec4 mvPosition = modelViewMatrix * vec4(pos, 1.0);
    vViewPosition = -mvPosition.xyz;
}
