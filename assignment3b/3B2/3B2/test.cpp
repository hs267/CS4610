

/*
#include <GL/glew.h>        // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

#include "InitShader.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>

#define MESH_FILE "/Users/thehanemperor/Desktop/SCHOOL/4610SP2018/assignment3b/Assign3B/cube.obj"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//using namespace glm;
//using namespace std;
const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
int g_gl_width = 640;
int g_gl_height = 480;




glm::vec4 points[NumVertices];
glm::vec2 texcoords[NumVertices]{
    
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(0.0f, 0.0f),
    
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 1.0f),
    
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 0.0f),
    
    glm::vec2(0.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(0.0f, 1.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 1.0f)
    
};
glm::vec3   normals[NumVertices];
std::vector<GLfloat*> vertices;
std::vector<GLint> faces;


//light
glm::vec4 light_position(0.0, 0.0, 4.0, 0.0); // x y z w
glm::vec4 light_ambient(0.2, 0.2, 0.2, 1.0); // R G B A
glm::vec4 light_diffuse(1.0, 1.0, 1.0, 1.0);
glm::vec4 light_specular(1.0, 1.0, 1.0, 1.0);

glm::vec4 material_ambient(1.0, 0.0, 1.0, 1.0);
glm::vec4 material_diffuse(1.0, 0.8, 0.0, 1.0);
glm::vec4 material_specular(1.0, 0.8, 0.0, 1.0);
float  material_shininess = 100.0;






GLuint vao;

GLuint  ModelView, Projection;

glm::vec3 ls = glm::vec3(1.0, 1.0, 1.0);
GLfloat fov = 45.0, aspect = 1.0;
GLfloat  zNear = 0.1f, zFar = 100.0f;
GLuint  projection; // projection matrix uniform shader variable location
GLfloat s = 0.0;
glm::mat4 m(1.0);
bool lbutton_down = false;
GLfloat xcenter, ycenter, zcenter;
GLfloat scale;
double x_pre, y_pre, x, y, hrotate, vrotate;

GLfloat x_trans = 0, y_trans = 0, z_trans = 0;
// Vertices of a unit cube centered at origin, sides aligned with axes

int readfile(std::string addrstr)
{
    FILE *file1, *file2;
    file1 = fopen(addrstr.c_str(), "r"); //read
    float a, b, c, *arrayfloat, *arraycenter, *arrayx, *arrayy, *arrayz;
    GLint e, f, g, *arrayint;
    GLfloat min_x = FLT_MAX;    GLfloat min_y = FLT_MAX;    GLfloat min_z = FLT_MAX;
    GLfloat max_x = -10000.0;    GLfloat max_y = -10000.0;    GLfloat max_z = -10000.0;
    
    char v;
    int count = 0;
    while (!feof(file1))
    {
        v = fgetc(file1);
        if (v == 'v')
        {
            arrayfloat = new GLfloat[3];
            fscanf(file1, "%f%f%f", &a, &b, &c);
            arrayfloat[0] = a;
            arrayfloat[1] = b;
            arrayfloat[2] = c;
            //cout << a << " " << b << " " << c << endl;
            min_x = std::min(a, min_x);
            min_y = std::min(b, min_y);
            min_z = std::min(c, min_z);
            max_x = std::max(a, max_x);
            max_y = std::max(b, max_y);
            max_z = std::max(c, max_z);
            vertices.push_back(arrayfloat);
            
        }
        else if (v == 'f')
        {
            arrayint = new GLint[3];
            fscanf(file1, "%d%d%d", &e, &f, &g);
            faces.push_back(e);
            faces.push_back(f);
            faces.push_back(g);
            
        }
    }
    fclose(file1);
    
    xcenter = (max_x + min_x) / 2;
    ycenter = (max_y + min_y) / 2;
    zcenter = (max_z + min_z) / 2;
    
    scale = std::max(max_x - min_x, std::max(max_y - min_y, max_z - min_z));
    
    GLfloat *points;
    points = (GLfloat *)malloc(faces.size() * 3 * sizeof(GLfloat));
    GLfloat *normals;
    normals = (GLfloat *)malloc(faces.size() * 3 * sizeof(GLfloat));
    for (int i = 0; i < faces.size(); ++i) {
        points[i * 3] = ((GLfloat)vertices[faces[i] - 1][0]-xcenter)/scale;
        points[i * 3 + 1] = ((GLfloat)vertices[faces[i] - 1][1]-ycenter)/scale;
        points[i * 3 + 2] = ((GLfloat)vertices[faces[i] - 1][2]-zcenter)/scale;
        
    }
    for (int i = 0; i < faces.size(); i += 3) {
        GLint e = faces[i];
        GLint f = faces[i + 1];
        GLint g = faces[i + 2];
        glm::vec3 v1 = glm::vec3(vertices[e - 1][0], vertices[e - 1][1], vertices[e - 1][2]);
        glm::vec3 v2 = glm::vec3(vertices[f - 1][0], vertices[f - 1][1], vertices[f - 1][2]);
        glm::vec3 v3 = glm::vec3(vertices[g - 1][0], vertices[g - 1][1], vertices[g - 1][2]);
        glm::vec3 n = glm::triangleNormal(v1, v2, v3);
        normals[count] = n.x; count++;
        normals[count] = n.y; count++;
        normals[count] = n.z; count++;
        normals[count] = n.x; count++;
        normals[count] = n.y; count++;
        normals[count] = n.z; count++;
        normals[count] = n.x; count++;
        normals[count] = n.y; count++;
        normals[count] = n.z; count++;
    }
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint points_vbo;
    GLuint texcoords_vbo;
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * faces.size() * sizeof(GLfloat), points,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    GLuint normals_vbo;
    glGenBuffers(1, &normals_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * faces.size() * sizeof(GLfloat), normals,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    
    //slides
    glGenBuffers(1, &texcoords_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, texcoords_vbo);
    glBufferData(GL_ARRAY_BUFFER, 36*2 * sizeof(GLfloat), vTexCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    //slides
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, 36*3 * sizeof(GLfloat), points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    

    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    return faces.size();
}
*/


// i