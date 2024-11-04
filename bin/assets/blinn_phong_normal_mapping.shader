#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 textureCoordinates;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec3 FragPos;
out vec2 TextureCoordinates;
out vec3 Normal;
out mat3 TBN;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main() {
    FragPos = vec3(u_Model * vec4(pos, 1.0f));
    Normal = mat3(transpose(inverse(u_Model))) * normals;
    
    vec3 T = normalize(mat3(u_Model) * tangent);
    vec3 B = normalize(mat3(u_Model) * bitangent);
    vec3 N = normalize(mat3(u_Model) * normals);
    TBN = transpose(mat3(T, B, N));

    TextureCoordinates = textureCoordinates;
    gl_Position = u_ViewProjection * u_Model * vec4(pos, 1.0f);
}

#shader fragment
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoordinates;
in mat3 TBN;

uniform struct Material {
    float shininess;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D metallic;
    sampler2D emissive;
    sampler2D normalMap;
    sampler2D occlusion;
    bool hasSpecular;
    bool hasMetallic;
    bool hasEmissive;
    bool hasNormalMap;
    bool hasOcclusion;
};

uniform Material material;

// Point light uniforms
uniform vec3 u_pointLightPos;
uniform vec3 u_pointLightColor;
uniform float u_pointLightIntensity;

// Directional light uniforms
uniform vec3 u_directionalLightDir;
uniform vec3 u_directionalLightColor;
uniform float u_directionalLightIntensity;

uniform vec3 u_ViewPos;
out vec4 FragColor;

void main() {
    // Normalize the normal vector
    vec3 norm = normalize(Normal);
    if (material.hasNormalMap) {
        norm = texture(material.normalMap, TextureCoordinates).rgb;
        norm = normalize(norm  * 2.0f - 1.0f);
    }

    // Sample from the diffuse texture
    vec4 diffuseTextureColor = texture(material.diffuse, TextureCoordinates);

    // Initialize specular, emissive, and metallic colors
    vec3 specularTextureColor = vec3(0.0f);
    if (material.hasSpecular) {
        specularTextureColor = texture(material.specular, TextureCoordinates).rgb;
    }
    
    vec3 emissiveColor = vec3(0.0f);
    if (material.hasEmissive) {
        emissiveColor = texture(material.emissive, TextureCoordinates).rgb;
    }

    vec3 metallicColor = vec3(0.0f);
    if (material.hasMetallic) {
        metallicColor = texture(material.metallic, TextureCoordinates).rgb;
    }

    vec3 occlusionColor = vec3(1.0f);
    if (material.hasOcclusion) {
        occlusionColor = texture(material.occlusion, TextureCoordinates).rgb;
    }

    // ----- Point Light Calculations -----
    // Ambient lighting for point light
    vec3 ambientPoint = 0.1f * u_pointLightColor;

    // Diffuse lighting for point light
    vec3 pointLightDir = TBN * normalize(u_pointLightPos - FragPos);
    float diffPoint = max(dot(norm, pointLightDir), 0.0f);
    vec3 diffusePoint = diffPoint * u_pointLightColor;

    // Specular lighting (Blinn-Phong) for point light
    vec3 viewDir = TBN * normalize(u_ViewPos - FragPos);
    vec3 halfwayDirPoint = normalize(pointLightDir + viewDir);
    float specPoint = pow(max(dot(norm, halfwayDirPoint), 0.0f), material.shininess);
    vec3 specularPoint = specPoint * specularTextureColor * u_pointLightColor * metallicColor * occlusionColor;

    // Attenuation for point light
    float distancePoint = length(u_pointLightPos - FragPos);
    float attenuationPoint = 1.0f / (distancePoint * distancePoint); // Quadratic attenuation

    // ----- Combine point light components -----
    vec3 pointLightResult = (ambientPoint + diffusePoint + specularPoint) * u_pointLightIntensity * attenuationPoint;

    // ----- Directional Light Calculations -----
    // Ambient lighting for directional light
    vec3 ambientDir = 0.1 * u_directionalLightColor;

    // Diffuse lighting for directional light
    vec3 dirLightDir = normalize(-u_directionalLightDir);
    float diffDir = max(dot(norm, dirLightDir), 0.0f);
    vec3 diffuseDir = diffDir * u_directionalLightColor;

    // Specular lighting (Blinn-Phong) for directional light
    vec3 halfwayDirDir = normalize(dirLightDir + viewDir);
    float specDir = pow(max(dot(norm, halfwayDirDir), 0.0f), material.shininess);
    vec3 specularDir = specDir * specularTextureColor * u_directionalLightColor * metallicColor * occlusionColor;

    // ----- Combine directional light components -----
    vec3 directionalLightResult = (ambientDir + diffuseDir + specularDir) * u_directionalLightIntensity;

    // ----- Combine Both Light Sources -----
    vec3 lightingResult = pointLightResult + directionalLightResult;

    // Combine lighting with texture color and emissive color
    vec4 finalColor = vec4(lightingResult, 1.0f) * diffuseTextureColor + vec4(emissiveColor, 1.0f); // * vec4(occlusionColor, 1.0f)
    
    if (material.hasNormalMap) {
        vec3 normalMapSample = texture(material.normalMap, TextureCoordinates).rgb;
        FragColor = vec4(normalMapSample, 1.0);
    }
    else {
        FragColor = vec4(norm, 1.0);
    }

    FragColor = finalColor;
}
