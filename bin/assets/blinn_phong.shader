#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 textureCoordinates;
out vec3 FragPos;
out vec3 Normal;
out vec2 TextureCoordinates;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

void main() {
    FragPos = vec3(u_Model * vec4(pos, 1.0));
    Normal = mat3(transpose(inverse(u_Model))) * normals;
    gl_Position = u_ViewProjection * u_Model * vec4(pos, 1.0);
    TextureCoordinates = textureCoordinates;
}

#shader fragment
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoordinates;

uniform struct Material {
    float shininess;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D metallic;
    sampler2D emissive;
    bool hasSpecular;
    bool hasMetallic;
    bool hasEmissive;

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

    // Sample from the specular texture, emissive texture and metallic texture
    vec4 diffuseTextureColor = texture(material.diffuse, TextureCoordinates);
    vec3 specularTextureColor = vec3(0.0f);
    if (material.hasSpecular) {
        vec4 specularTextureColor = texture(material.specular, TextureCoordinates);
    }
    vec3 emissiveColor = vec3(0.0f);  // Default to black
    if (material.hasEmissive) {
        emissiveColor = texture(material.emissive, TextureCoordinates).rgb;
    }
    vec3 metallicColor = vec3(0.0f);
    if (material.hasMetallic) {
        vec3 metallicColor = texture(material.metallic, TextureCoordinates).rgb;
    }

    // ----- Point Light -----
    // Ambient lighting for point light
    vec3 ambientPoint = 0.1 * u_pointLightColor;

    // Diffuse lighting for point light
    vec3 pointLightDir = normalize(u_pointLightPos - FragPos);
    float diffPoint = max(dot(norm, pointLightDir), 0.0);
    vec3 diffusePoint = diffPoint * u_pointLightColor;

    // Specular lighting (Blinn-Phong) for point light
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 halfwayDirPoint = normalize(pointLightDir + viewDir);
    float specPoint = pow(max(dot(norm, halfwayDirPoint), 0.0), material.shininess);
    // Use the specular texture color and the metallic texture color
    vec3 specularPoint = specPoint * specularTextureColor.rgb * u_pointLightColor * metallicColor;

    // Attenuation for point light
    float distancePoint = length(u_pointLightPos - FragPos);
    float attenuationPoint = 1.0 / (distancePoint * distancePoint); // Quadratic attenuation

    // ----- Combine point light components -----
    vec3 pointLightResult = (ambientPoint + diffusePoint + specularPoint) * u_pointLightIntensity * attenuationPoint;

    // ----- Directional Light -----
    // Ambient lighting for directional light
    vec3 ambientDir = 0.1 * u_directionalLightColor;

    // Diffuse lighting for directional light
    vec3 dirLightDir = normalize(-u_directionalLightDir);
    float diffDir = max(dot(norm, dirLightDir), 0.0);
    vec3 diffuseDir = diffDir * u_directionalLightColor;

    // Specular lighting (Blinn-Phong) for directional light
    vec3 halfwayDirDir = normalize(dirLightDir + viewDir);
    float specDir = pow(max(dot(norm, halfwayDirDir), 0.0), material.shininess);
    vec3 specularDir = specDir * specularTextureColor.rgb * u_directionalLightColor;

    // ----- Combine directional light components -----
    vec3 directionalLightResult = (ambientDir + diffuseDir + specularDir) * u_directionalLightIntensity;

    // ----- Combine Both Light Sources -----
    vec3 lightingResult = pointLightResult + directionalLightResult;

    // Combine lighting with texture color
    vec4 finalColor = vec4(lightingResult, 1.0) * diffuseTextureColor + vec4(emissiveColor, 1.0);

    FragColor = finalColor;
}
