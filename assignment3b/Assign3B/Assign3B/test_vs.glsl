#version 410

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;
out vec4 color;


void main()
{
    // Transform vertex  position into eye coordinates
    
    
    vec3 pos = (ModelView * vec4(vPosition, 1.0)).xyz; // Transform vertex  position into eye coordinates
    vec3 L = normalize((ModelView*LightPosition).xyz - pos); //unit vector in direction  of light source
    vec3 N = normalize(ModelView*vec4(vNormal, 0.0)).xyz; // // Transform vertex normal into eye coordinates
    float Kd = max(dot(L, N), 0.0);
    vec4  diffuse = Kd*DiffuseProduct;
    vec4 ambient = AmbientProduct;
    
    vec3 E = normalize(-pos);   // normalized vector in the direction of the viewer
    vec3 H = normalize(L + E); // halfway vector
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4  specular = Ks * SpecularProduct;
    if (dot(L, N) < 0.0) {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
    }

    color = ambient + diffuse + specular;
    color.a = 1.0;
    
    gl_Position = Projection * ModelView * vec4(vPosition, 1.0);


    
}
