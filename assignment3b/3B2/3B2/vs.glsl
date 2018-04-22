#version 410

in  vec4 vPosition;
in  vec4 vColor;
out vec4 color;

uniform mat4 mvp;

void main() 
{

    color = vColor;
    gl_Position = mvp * vPosition;
} 
