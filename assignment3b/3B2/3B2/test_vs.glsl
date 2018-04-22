#version 410

layout(location = 0) in vec3 vPosition;
//layout(location = 1) in vec2 vt;
uniform mat4 ModelView;
uniform mat4 Projection;
out vec2 texture_coordinates;

in vec3 vertex_position;
in vec2 vt; // per-vertex texture co-ords
//out vec2 texture_coordinates;
// other variables


void main()
{
    texture_coordinates =vt;
    gl_Position = Projection * ModelView * vec4(vPosition, 1.0);
}


