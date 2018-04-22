//
//  main.cpp
//  Part A
// Simple.cpp
// Generate a square on a solid background


#include "GL/glew.h"
#include <stdio.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"

GLuint shader_programme;
GLuint vao;
//----------------------------------------------------------------------------
void init( void )
{
    
    GLuint vert_shader, frag_shader;
    /* GL shader programme object [combined, to link] */
    
    // Specifiy the vertices for a triangle
    GLfloat points[] = { 0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5, -0.5,0.5 };
    
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
    
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
    "    FragColor = vec4( 0.0, 1.0, 0.0, 1.0 );"
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


//----------------------------------------------------------------------------

int main( int argc, char **argv )
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
    
    
    do{
        glClear(GL_COLOR_BUFFER_BIT);     // clear the window
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3 );
        glDrawArrays(GL_TRIANGLES, 3, 3);// draw the points
        /* update other events like input handling */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    while (!glfwWindowShouldClose(window));
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
    
}

