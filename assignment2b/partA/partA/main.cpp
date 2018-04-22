//
//  main.cpp
//  partA
//
//  Created by Han Song on 3/13/18.
//  Copyright © 2018 Han Song. All rights reserved.
//

#include "GL/glew.h"
#include <stdio.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"

const int NumPoints = 5000;
GLfloat points[5000000];
GLuint shader_programme;
GLuint vao;
GLuint buffer;
int count = 0;
int w = 640;

//----------------------------------------------------------------------------
void init(void)
{
    GLuint vert_shader, frag_shader;
    
    // Create and initialize a buffer object
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    
    // Create a vertex array object
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // "attribute #0 is created from every 2 variables in the above buffer, of type
    // float (i.e. make me vec2s)"
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
    // Load shaders and use the resulting shader program
    const char *vertex_shader = "#version 410\n"
    "in vec4 vPosition;"
    "void main () {"
    "    gl_Position = vPosition;"
    "}";
    /* the fragment shader colours each fragment (pixel-sized area of the
     triangle) */
    const char *fragment_shader = "#version 410\n"
    "out vec4 FragColor;"
    "void main () {"
    "    FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );"
    "}";
    
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vertex_shader, NULL);
    glCompileShader(vert_shader);
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader, NULL);
    glCompileShader(frag_shader);
    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, frag_shader);
    glAttachShader(shader_programme, vert_shader);
    glLinkProgram(shader_programme);
}

void drawGasket(float x, float y) {
    float x_w = (2.0f * x / w) - 1.0f;
    float y_w = 1.0f - (2.0f * y) / w;
    
    //GLfloat vertices[] = { -0.1f, -0.1f,  0.0f, 0.1f,  0.1f, -0.1f };
    GLfloat vertices[] = { (x_w - 1.0f) / 2, (y_w - 1.0f) / 2,  x_w / 2, (y_w + 1.0f) / 2,  (x_w + 1.0f) / 2, (y_w - 1.0f) / 2 };
    
    points[count] = x_w + 0.25;
    points[count + 1] = y_w - 0.5;
    
    for (int i = count + 2; i < (NumPoints * 2) + count; i += 2) {
        int j = rand() % 3;   // pick a vertex at random
        // Compute the point halfway between the selected vertex
        //  and the previous point
        points[i] = (points[i - 2] + vertices[2 * j]) / 2.0;
        points[i + 1] = (points[i - 1] + vertices[2 * j + 1]) / 2.0;
    }
    
    //glGenBuffers( 1, &buffer );
    
    //glGenVertexArrays(1, &vao);
    
    //glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // "attribute #0 is created from every 2 variables in the above buffer, of type
    // float (i.e. make me vec2s)"
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    count += (NumPoints);
}

void mymouse(GLFWwindow* window, int button, int action, int mods) {
    if (GLFW_PRESS == action) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        drawGasket(x, y);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
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
    //#ifdef APPLE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //#endif
    window = glfwCreateWindow(640, 640, "Sierpinski Gasket", NULL, NULL);
    
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
    
    glfwSetMouseButtonCallback(window, mymouse);
    
    do {
        glClear(GL_COLOR_BUFFER_BIT);     // clear the window
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, count);    // draw the points
        /* update other events like input handling */
        glfwPollEvents();
        glfwSwapBuffers(window);
    } while (!glfwWindowShouldClose(window));
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
    
}

