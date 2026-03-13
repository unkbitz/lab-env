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

uniform sampler2D gTexture;

void main()
{
    FragColor = texture(gTexture, TexCoord);
}