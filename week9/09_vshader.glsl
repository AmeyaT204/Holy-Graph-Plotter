#version 330 core

in vec4 vPosition;
in vec4 vNormal;

uniform mat4 uModelViewMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

out vec3 fNormal;
out float fHeight;
out vec3 fPos;

void main()
{
    fHeight = vPosition.y;
    fNormal = normalize(normalMatrix * vec3(vNormal));
    vec4 eyePos = uModelViewMatrix * vPosition;
    fPos = eyePos.xyz;
    gl_Position = uModelViewMatrix * vPosition;
}
