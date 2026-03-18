#shader vertex
#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    TexCoord = aTexCoord;
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

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 viewPos;
uniform float shininess;

void main()
{
    vec3 fragPos = texture(gPosition, TexCoord).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoord).xyz);
    vec3 albedo = texture(gAlbedo, TexCoord).rgb;
    float specularStrength = texture(gSpecular, TexCoord).r;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 L = normalize(-lightDir);

    // ambient
    //vec3 ambient = 0.02 * albedo;
    vec3 ambient = vec3(0,0,0);

    // diffuse
    float diff = max(dot(normal, L), 0.0);
    vec3 diffuse = diff * albedo * lightColor;

    // specular (Blinn-Phong)
    vec3 halfway = normalize(L + viewDir);
    float spec = pow(max(dot(normal, halfway), 0.0), shininess);
    vec3 specular = spec * specularStrength * lightColor;

    vec3 result = ambient + (diffuse + specular) * lightIntensity;

    FragColor = vec4(result, 1.0);
}