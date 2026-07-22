#version 330 core

in vec3 fNormal;
in float fHeight;
in vec3 fPos;

uniform float minHeight;
uniform float maxHeight;

out vec4 FragColor;

vec3 heightToColor(float h)
{
    float t = clamp((h - minHeight) / (maxHeight - minHeight), 0.0, 1.0);
    vec3 low  = vec3(0.15, 0.25, 0.9);
    vec3 mid  = vec3(0.2, 0.85, 0.3);
    vec3 high = vec3(0.9, 0.2, 0.15);
    if (t < 0.5) return mix(low, mid, t * 2.0);
    return mix(mid, high, (t - 0.5) * 2.0);
}

void main()
{
    vec3 baseColor = heightToColor(fHeight);

    vec3 lightPos = vec3(3.0, 5.0, 3.0);
    vec3 N = normalize(fNormal);
    vec3 L = normalize(lightPos - fPos);
    vec3 V = normalize(-fPos);
    vec3 R = reflect(-L, N);

    float ambient = 0.25;
    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(V, R), 0.0), 32.0);

    vec3 result = baseColor * (ambient + diff) + vec3(0.3) * spec;
    FragColor = vec4(result, 1.0);
}