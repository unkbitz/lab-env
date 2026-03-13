#shader vertex
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

out vec2 TexCoord;

void main()
{
    TexCoord = aUV;
    gl_Position = vec4(aPos, 0.0, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform vec4 lightPos;
uniform vec4 lightColor;
uniform float lightIntensity;

uniform vec4 viewPos;
uniform float shininess;

void main()
{
    vec3 fragPos = texture(gPosition, TexCoord).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoord).xyz);
    vec3 albedo = texture(gAlbedo, TexCoord).rgb;
    vec3 specularColor = texture(gSpecular, TexCoord).rgb;

    vec3 lightDir = normalize(lightPos.xyz - fragPos);
    vec3 viewDir = normalize(viewPos.xyz - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);

    float distanceToLight = length(lightPos.xyz - fragPos);
    float attenuation = 1.0 / (1.0 + 0.035 * distanceToLight + 0.44 * distanceToLight * distanceToLight);

    vec3 ambient = 0.3 * albedo;
    vec3 diffuse = 0.8 * diff * albedo * lightColor.xyz;
    vec3 specular = 0.25 * spec * specularColor * lightColor.xyz;

    vec3 result = (diffuse + specular) * lightIntensity * attenuation;

    FragColor = vec4(result, 1.0);
}