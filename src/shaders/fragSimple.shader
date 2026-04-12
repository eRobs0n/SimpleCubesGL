#version 330 core

out vec4 FragColor;

in vec3 Normal;

uniform vec3 albedoColor;

void main()
{
    vec3 norm = normalize(Normal);

    // "свет сверху"
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));

    float diff = max(dot(norm, lightDir), 0.0);

    // немного базового освещения
    float lighting = 0.2 + 0.8 * diff;

    vec3 color = albedoColor * lighting;

    FragColor = vec4(color, 1.0);
}