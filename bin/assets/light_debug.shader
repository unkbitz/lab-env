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
uniform vec4 u_Color;

void main()
{
    FragColor = u_Color;
}