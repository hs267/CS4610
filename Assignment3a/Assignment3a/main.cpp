// RotateCube.cpp
// Generate a rotating cube on a solid background
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "InitShader.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/normal.hpp>


GLuint vao;
int Index = 0;
GLuint program;
const int NumVertices = 36;
bool isPressed = false;
double oldX;
double oldY;
double hrotate = 0;
double vrotate = 0;
glm::mat4 model = glm::mat4(1.0);
glm::vec3 eye(0.0f, 0.0f, 2.0f);
glm::vec3 at(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0, 1.0f, 0.0f);
glm::vec4 points[NumVertices];
glm::vec4 colors[NumVertices];
//----------------------------------------------------------------------------

glm::vec4 vertices[8] = {
    glm::vec4(-0.5, -0.5, 0.5, 1.0),
    glm::vec4(-0.5, 0.5, 0.5, 1.0),
    glm::vec4(0.5, 0.5, 0.5, 1.0),
    glm::vec4(0.5, -0.5, 0.5, 1.0),
    glm::vec4(-0.5, -0.5, -0.5, 1.0),
    glm::vec4(-0.5, 0.5, -0.5, 1.0),
    glm::vec4(0.5, 0.5, -0.5, 1.0),
    glm::vec4(0.5, -0.5, -0.5, 1.0)
};

glm::vec4 vertex_colors[8] = {
    glm::vec4(0.0, 0.0, 0.0, 1.0),  // black
    glm::vec4(1.0, 0.0, 0.0, 1.0),  // red
    glm::vec4(1.0, 1.0, 0.0, 1.0),  // yellow
    glm::vec4(0.0, 1.0, 0.0, 1.0),  // green
    glm::vec4(0.0, 0.0, 1.0, 1.0),  // blue
    glm::vec4(1.0, 0.0, 1.0, 1.0),  // magenta
    glm::vec4(1.0, 1.0, 1.0, 1.0),  // white
    glm::vec4(0.0, 1.0, 1.0, 1.0)   // cyan
};

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };


GLuint  mvpi;

void quad(int a, int b, int c, int d)
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}
void
colorcube()
{
    quad(1, 0, 3, 2);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(6, 5, 1, 2);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);
}

void init(void)
{
    colorcube();
    
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    
    // Load shaders and use the resulting shader program
    GLuint program = InitShader("/Users/thehanemperor/Desktop/SCHOOL/4610SP2018/Assignment3a/Assignment3a/vshader36.glsl", "/Users/thehanemperor/Desktop/SCHOOL/4610SP2018/Assignment3a/Assignment3a/fshader36.glsl");
    glUseProgram(program);
    
    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          NULL);
    
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
                          NULL);
    
    mvpi = glGetUniformLocation(program, "mvp");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void mymouse(GLFWwindow* window, int button, int action, int mods)
{
    
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (GLFW_PRESS == action) {
            isPressed = true;
            glfwGetCursorPos(window, &oldX, &oldY);
        }
        else if (GLFW_RELEASE == action) {
            isPressed = false;
            hrotate = 0;
            vrotate = 0;
        }
    }
}
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    if (isPressed) {
        hrotate += 0.01*(xpos - oldX);
        vrotate += 0.01*(ypos - oldY);
    }
    model = glm::rotate(model, GLfloat(hrotate), glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, GLfloat(vrotate), glm::vec3(1.0, 0.0, 0.0));
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    printf("%d", yoffset);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, yoffset > 0 ? 0.1f : -0.1f));
}





void mykey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        if (key == GLFW_KEY_T) {
            model = glm::perspective(glm::radians(300.0f), 3.0f, 0.5f, 80.f);
            
            
        }
        if (key == GLFW_KEY_G) {
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.1f));
        }
        if (key == GLFW_KEY_W) {
            model = glm::translate(model, glm::vec3(0.0f, +0.1f, 0.0f));
        }
        if (key == GLFW_KEY_A) {
            model = glm::translate(model, glm::vec3(-0.1f, 0.0f, 0.0f));
        }
        if (key == GLFW_KEY_S) {
            model = glm::translate(model, glm::vec3(0.0f, -0.1f, 0.0f));
        }
        if (key == GLFW_KEY_D) {
            model = glm::translate(model, glm::vec3(+0.1f, 0.0f, 0.0f));
        }
        if (key == GLFW_KEY_H) {
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.1f));
        }
        if (key == GLFW_KEY_X) {
            model = glm::rotate(model, (glm::mediump_float)10, glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (key == GLFW_KEY_Y) {
            model = glm::rotate(model, (glm::mediump_float)10, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (key == GLFW_KEY_Z) {
            model = glm::rotate(model, (glm::mediump_float)10, glm::vec3(0.0f, 0.0f, 1.0f));
        }
        if (key == GLFW_KEY_B) {
            
            
            model = glm::scale(glm::mat4(2.0f), glm::vec3(1.0f));
            if (key == GLFW_KEY_B) {
                
                glm::mat4 model = glm::mat4(1.0);
                model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
                
                glm::vec3 eye(0.0f, 0.0f, 2.0f);
                glm::vec3 at(0.0f, 0.0f, 0.0f);
                glm::vec3 up(0.0, 1.0f, 0.0f);
                glm::mat4 view = glm::lookAt(eye, at, up);
                
                glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
                glm::mat4 mvp = Projection * view * model;
                
            }
            
            
            
        }
        
    }
}
//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    GLFWwindow *window = NULL;
    const GLubyte *renderer;
    const GLubyte *version;
    /* start GL context and O/S window using the GLFW helper library */
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    /* We must specify 3.2 core if on Apple OS X -- other O/S can specify
     anything here. I defined 'APPLE' in the makefile for OS X
     
     Remove the #ifdef #endif and play around with this - you should be starting
     an explicit version anyway, and some non-Apple drivers will require this too.
     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(640, 640, "simple", NULL, NULL);
    
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    /* start GLEW extension handler */
    glewExperimental = GL_TRUE;
    glewInit();
    renderer = glGetString(GL_RENDERER); /* get renderer string */
    version = glGetString(GL_VERSION);     /* version as a string */
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    init();
    
    glfwSetKeyCallback(window, mykey);
    glfwSetMouseButtonCallback(window, mymouse);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    //
    
    
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // clear the window
        
        
        
        glm::mat4 view = glm::lookAt(eye, at, up);
        glm::mat4 Projection = glm::perspective(glm::radians(450.0f), 1.0f, 0.1f, 100.f);
        glm::mat4 mvp = Projection * view * model;
        
        glUniformMatrix4fv(mvpi, 1, GL_FALSE, glm::value_ptr(mvp));
        
        glDrawArrays(GL_TRIANGLES, 0, NumVertices);    // draw the points
        /* update other events like input handling */
        glfwPollEvents();
        glfwSwapBuffers(window);
    } while (!glfwWindowShouldClose(window));
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
    
}


