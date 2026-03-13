#shader vertex
#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

void main()
{
    vec4 worldPos = u_Model * aPos;
    FragPos = worldPos.xyz;
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    TexCoord = aUV;

    gl_Position = u_ViewProjection * worldPos;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedo;
layout(location = 3) out vec4 gSpecular;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

uniform int useDiffuseTexture;
uniform vec4 baseColor;

uniform int useSpecularTexture;
uniform vec4 specularColor;

void main()
{
    gPosition = vec4(FragPos, 1.0);
    gNormal   = vec4(normalize(Normal), 1.0);

    vec4 diffuseColor = baseColor;
    if (useDiffuseTexture == 1)
    {
        diffuseColor = texture(diffuseTex, TexCoord);
    }

    vec4 specColor = specularColor;
    if (useSpecularTexture == 1)
    {
        specColor = texture(specularTex, TexCoord);
    }

    gAlbedo = diffuseColor;
    gSpecular = specColor;
}