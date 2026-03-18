#shader vertex
#version 330 core

layout(location = 0) in vec4 aPos;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * u_Model * aPos;
}

#shader fragment
#version 330 core

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gSpecular;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform float lightRadius;

uniform vec3 viewPos;
uniform float shininess;

void main()
{
    ivec2 pixel = ivec2(gl_FragCoord.xy);

    vec4 posSample    = texelFetch(gPosition, pixel, 0);
    vec4 normSample   = texelFetch(gNormal, pixel, 0);
    vec4 albedoSample = texelFetch(gAlbedo, pixel, 0);
    vec4 specSample   = texelFetch(gSpecular, pixel, 0);

    if (posSample.a < 0.5)
        discard;

    vec3 fragPos = posSample.xyz;
    vec3 normal  = normalize(normSample.xyz);
    vec3 albedo  = albedoSample.rgb;
    float specularStrength = specSample.r;

    vec3 L = lightPos - fragPos;
    float distanceToLight = length(L);

    float fadeStart = lightRadius * 0.6;
    float fadeEnd   = lightRadius;

    if (distanceToLight > fadeEnd)
        discard;

    vec3 lightDir = L / max(distanceToLight, 0.0001);
    vec3 viewDir  = normalize(viewPos - fragPos);
    float w = fwidth(distanceToLight);
    float attenuation = 1.0 - smoothstep(fadeStart - w, fadeEnd + w, distanceToLight);
    attenuation = attenuation * attenuation * attenuation;

    float diff = max(dot(normal, lightDir), 0.0);

    float spec = 0.0;
    if (diff > 0.0)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    }

    vec3 diffuse  = diff * albedo * lightColor;
    vec3 specular = 0.1 * spec * specularStrength * lightColor;

    vec3 result = (diffuse + specular) * lightIntensity * attenuation;
    FragColor = vec4(result, 1.0);
}