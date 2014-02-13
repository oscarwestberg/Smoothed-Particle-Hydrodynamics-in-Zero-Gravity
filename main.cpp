//
//  main.cpp
//  Smoothed Particle Hydrodynamics in Zero Gravity
//
//  Created by Oscar Westberg on 2014-01-24.
//  Copyright (c) 2014 Group 7 @ Linköpings University, course TNM085. All rights reserved.
//

// Link statically with GLEW
//#define GLEW_STATIC

// Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "ParticleSystem.h"

/*
    TODO:
    Shader loader function in Shader class with bug test - DONE
    Rotation from input affecting the view matrix
    Create a way to draw points in space for testing - DONE
    Find a way to change the values in the vertex  buffer - DONE
    Update the way time is calculated - DONE
    XML to load settings? Particle amount, resolution etc?
    Implement Metaballs/Isosurface 2D - DONE
    -II- in 3D 
    Find a way to use the MVP matrix in the shader
 
    RULES:
    Keep it simple
    Keep it consistent
	Rösta vänster
*/

GLuint shaderProgram;
glm::vec2 velocity;
float previousTime = 0;
const float width = 800, height = 600;
ParticleSystem particleSystem;

// Updates all the views using user input
void update()
{
    // Update the particle system
    float time = (float)glfwGetTime();
    particleSystem.updateParticles(time-previousTime);
    
    // -----------------------------------
    // Model Matrix - currently ignored
    // -----------------------------------
    
    // Create a 4x4 matrix
    glm::mat4 trans;
    GLint uniTrans = glGetUniformLocation(shaderProgram, "M");
    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));
    
    // -----------------------------------
    // View Matrix
    // -----------------------------------
    
    // Position of the camera, Point centered on-screen and the Up axis.
    glm::mat4 view = glm::lookAt(
                                 glm::vec3(1.2f, 1.2f, 1.2f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 1.0f)
                                 );
    GLint uniView = glGetUniformLocation(shaderProgram, "V");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
    
    // -----------------------------------
    // Projection Matrix
    // -----------------------------------
    
    // Vertical field-of-view, Aspect ratio, Near and Far planes.
    glm::mat4 proj = glm::perspective(45.0f, width / height, 0.1f, 20.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "P");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
    
    previousTime = time;
}

// Draw to the buffer and give vertices colors
void render()
{
    // Send positions to shader by putting the xzy values in seperate GLfloats
    GLfloat v1[MAX_PARTICLES];
    GLfloat v2[MAX_PARTICLES];
    GLfloat v3[MAX_PARTICLES];
    
    for(int i = 0; i < MAX_PARTICLES; i++){
        v1[i] = particleSystem.Particles[i].pos.x;
        v2[i] = particleSystem.Particles[i].pos.y;
        v3[i] = particleSystem.Particles[i].pos.z;
    }

    //glUniform3fv(glGetUniformLocation(shaderProgram, "positions"), MAX_PARTICLES, (v1,v2,v3));
    glm::vec3 particlePositions[MAX_PARTICLES];
    
    // Send positions to shader by putting the xzy values in seperate GLfloats
    for(int i = 0; i < MAX_PARTICLES; i++){
        particlePositions[i] = glm::vec3(particleSystem.Particles[i].pos);
    }
    
	GLint myLoc = glGetUniformLocation(shaderProgram, "positions");
    glUniform3fv(myLoc, MAX_PARTICLES, &particlePositions[0][0]);

    // Clear the screen
    // Entire system is drawn to a square that covers the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main(int argc, char* argv[])
{
    // -----------------------------------
    // Initialize
    // -----------------------------------
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    // Initialize the particle system and have it load into buffers
    particleSystem.initParticleSystem();
    
    // -----------------------------------
    // Create buffers
    // -----------------------------------
    
    // Create Vertex Array Object -  these store the links between VBOs and the raw vertex data
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Used to upload data to the graphics card
    // OpenGL handles the memory, so all we need is an int pointing to it
    // Vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo); // Generate buffer
    
    GLfloat vertices[] = {
        -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, // Top-left
        1.0f,  1.0f, 0.0f, 1.0f, 0.0f, // Top-right
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -1.0f, -1.0f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // Upload data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Upload data
    
    // Create an element array
    GLuint ebo;
    glGenBuffers(1, &ebo);
    
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    
    // --------------------------------------------
    // Compile and link shaders
    // --------------------------------------------
    
    // Load and compile shaders
    glEnable(GL_BLEND);
    shaderProgram = LoadShaders( "vertexshader.vert", "fragmentshader.frag" );
    
    // Fragment shaders can have several outputs, so we have to define which one we're looking for
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    
    // Actually use our shader program
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    
    // Tell OpenGL how our vertices are ordered
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    
    // Specify how the data is gathered from the vertex array
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    
    // --------------------------------------------
    // Main loop
    // --------------------------------------------
    
    while(!glfwWindowShouldClose(window)){
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            velocity.x += 1;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            velocity.x -= 1;
        
        glfwPollEvents();
        update();
        render();
        glfwSwapBuffers(window); // Swap buffers
    }
    
    // -----------------------------------
    // Clean up
    // -----------------------------------
    
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}