// Link statically with GLEW
#define GLEW_STATIC

// Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Shader sources
const GLchar* vertexSource =
"#version 150 core\n"
"in vec2 position;"
"void main() {"
"   gl_Position = vec4(position, 0.0, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 150 core\n"
"out vec4 outColor;"
"void main() {"
"   outColor = vec4(1.0, 1.0, 1.0, 1.0);"
"}";

int main(int argc, char* argv[])
{
    // -----------------------------------
    // Initialize glfw and glew
    // -----------------------------------
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    GLfloat vertices[] = {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };
    
    // -----------------------------------
    // Create buffers
    // -----------------------------------
    
    // Vao - these store the links between VBOs and the raw vertex data
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Used to upload data to the graphics card
    // OpenGL handles the memory, so all we need is an int pointing to it
    GLuint vbo; // Vertex buffer object
    glGenBuffers(1, &vbo); // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // Upload data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertex data to buffer
    
    // --------------------------------------------
    // Compile and link shaders
    // --------------------------------------------
    
    // Load and compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    
    // Connect the shaders, make them a unified program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Fragment shaders can have several outputs, so we have to define which one we're looking for
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    // Actually use our shader program
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    // Tell OpenGL how our vertices are ordered
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    // Specify how the data is gathered from the vertex array
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Check if shader was compiled correctly
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    fprintf(stderr, "Prog Info Log: %s\n", buffer);
    
    // --------------------------------------------
    // Main loop
    // --------------------------------------------
    
    
    while(!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        
        glfwPollEvents();
        
        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw a triangle from the 3 vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // Swap buffers
        glfwSwapBuffers(window);
    }
    
    // -----------------------------------
    // Clean up
    // -----------------------------------
    
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}