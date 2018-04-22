//
//  main.cpp
//  rotat
//
//  Created by Han Song on 3/13/18.
//  Copyright © 2018 Han Song. All rights reserved.
//

// RotateCube.cpp
// Generate a rotating cube on a solid background


#include "GL/glew.h"
#include <stdio.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"
#include "Angel.h"

typedef  vec4  point4;
typedef  vec4  color4;

GLuint vao;
int Index = 0;
const int NumVertices = 36;
int pause=0;
point4 points[NumVertices];
color4 colors[NumVertices];
//----------------------------------------------------------------------------






point4 vertices[8] = {
    point4(-0.5, -0.5, 0.5, 1.0),
    point4(-0.5, 0.5, 0.5, 1.0),
    point4(0.5, 0.5, 0.5, 1.0),
    point4(0.5, -0.5, 0.5, 1.0),
    point4(-0.5, -0.5, -0.5, 1.0),
    point4(-0.5, 0.5, -0.5, 1.0),
    point4(0.5, 0.5, -0.5, 1.0),
    point4(0.5, -0.5, -0.5, 1.0)
};

color4 vertex_colors[8] = {
    color4(0.0, 0.0, 0.0, 1.0),  // black
    color4(1.0, 0.0, 0.0, 1.0),  // red
    color4(1.0, 1.0, 0.0, 1.0),  // yellow
    color4(0.0, 1.0, 0.0, 1.0),  // green
    color4(0.0, 0.0, 1.0, 1.0),  // blue
    color4(1.0, 0.0, 1.0, 1.0),  // magenta
    color4(1.0, 1.0, 1.0, 1.0),  // white
    color4(0.0, 1.0, 1.0, 1.0)   // cyan
};

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };


GLuint  theta;  // The location of the "theta" shader uniform variable
void
quad(int a, int b, int c, int d)
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors),
                 NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    
    // Load shaders and use the resulting shader program
    GLuint program = InitShader("/Users/thehanemperor/Desktop/SCHOOL/4610SP2018/assignment2b/rotat/rotat/vshader36.glsl", "/Users/thehanemperor/Desktop/SCHOOL/4610SP2018/assignment2b/rotat/rotat/fshader36.glsl");
    glUseProgram(program);
    
    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0));
    
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(sizeof(points)));
    
    theta = glGetUniformLocation(program, "theta");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void mymouse(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            Axis = Xaxis;
        }
        if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            Axis = Yaxis;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            Axis = Zaxis;
        }
    }
}

void mykey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        // Press the key 'x' to start rotate in the x axis.
        if (key == GLFW_KEY_X) {
            Axis = Xaxis;
        }
        //Press the key 'y' to start rotate in the y axis.
        if (key == GLFW_KEY_Y) {
            Axis = Yaxis;
        }
        //Press the key 'z' to start rotate in the z axis.
        if (key == GLFW_KEY_Z) {
            Axis = Zaxis;
        }
        //Press the key 's' to stop the rotation.
        if (key == GLFW_KEY_S) {
            if (pause == 0) {
                pause = 1;
            }
            else if (pause == 1) {
                pause = 0;
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
    //view = glGetUniformLocation(shader_programme, "view");
    
    do{
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);     // clear the window
        if (pause == 0) {
        Theta[Axis] += 1.0;
        if (Theta[Axis] > 360.0) {
            Theta[Axis] -= 360.0;
        }
        }
        glUniform3fv(theta, 1, Theta);
        
        glDrawArrays(GL_TRIANGLES, 0, NumVertices);    // draw the points
        /* update other events like input handling */
        glfwPollEvents();
        glfwSwapBuffers(window);
    } while (!glfwWindowShouldClose(window));
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
    
}

