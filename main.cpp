//
//  main.cpp
//  Smoothed Particle Hydrodynamics in Zero Gravity
//
//  Created by Oscar Westberg on 2014-01-24.
//  Copyright (c) 2014 Group 7 @ Linköpings University, course TNM085. All rights reserved.
//

// Link statically with GLEW
#define GLEW_STATIC

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
    Implement Metaballs
 
    RULES:
    Keep it simple
    Keep it consistent
*/

GLuint shaderProgram;
glm::vec2 velocity;
float previousTime = 0;
const float width = 800, height = 600;
ParticleSystem particleSystem;

// Updates all the views using user input
void update()
{
    float time = (float)glfwGetTime();
    
    particleSystem.updateParticles(time-previousTime);
    
    // -----------------------------------
    // Model Matrix
    // -----------------------------------
    
    // Create a 4x4 matrix
    glm::mat4 trans;
//    trans = glm::rotate(
//                        trans,
//                        time * 180.0f,
//                        glm::vec3(0.0f, 0.0f, 1.0f)
//                        );
    // Change the shader variable
    GLint uniTrans = glGetUniformLocation(shaderProgram, "M");
    // Give it a value
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
    // In order to change the value of a shader variable, these functions have to be called
    GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
//    float time = (float)glfwGetTime();
//    glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);
    glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);
    
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw a points from the buffer
    glDrawArrays(GL_POINTS, 0, particleSystem.getParticleAmount());
}

int main(int argc, char* argv[])
{
    // -----------------------------------
    // Initialize glfw and glew
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
    
    // --------------------------------------------
    // Compile and link shaders
    // --------------------------------------------
    
    // Load and compile shaders
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
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
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
}