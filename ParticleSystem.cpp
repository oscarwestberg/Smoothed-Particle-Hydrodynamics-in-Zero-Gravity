//
//  ParticleSystem.cpp
//  Smoothed-Particle-Hydrodynamics-in-Zero-Gravity
//
//  Created by Oscar Westberg on 2014-01-29.
//  Copyright (c) 2014 Group 7. All rights reserved.
//

#include "ParticleSystem.h"
#include <math.h>

#define MAX_PARTICLES 300
#define MAX_VERTICES MAX_PARTICLES*3

struct Particle
{
    glm::vec3 pos;
    glm::vec3 vel;
    float LifetimeMillis; 
};

Particle Particles[MAX_PARTICLES];

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void ParticleSystem::initParticleSystem()
{
    GLfloat points[MAX_VERTICES];
    
    //srand(time(NULL)); // Not needed?
    
    // Give all the particles an initial value
    for(int i = 0; i < MAX_PARTICLES; i++){
        Particles[i].pos.x = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
        Particles[i].pos.y = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
        Particles[i].pos.z = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
        
        points[i*3] = Particles[i].pos.x;
        points[i*3+1] = Particles[i].pos.y;
        points[i*3+2] = Particles[i].pos.z;
    }
    
    // -----------------------------------
    // Create buffers
    // -----------------------------------
    
    // Vao - these store the links between VBOs and the raw vertex data
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Used to upload data to the graphics card
    // OpenGL handles the memory, so all we need is an int pointing to it
    // Vertex buffer object
    glGenBuffers(1, &vbo); // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // Upload data
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW); // Copy vertex data to buffer
}

int ParticleSystem::getParticleAmount()
{
    return MAX_VERTICES;
}

void ParticleSystem::updateParticles(float deltaTime)
{
    GLfloat* data;
    data = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
    
    // Change the values in the buffer
    if (data != (GLfloat*) NULL) {
        for(int i = 0; i < MAX_PARTICLES; ++i ){
            glm::vec3 force;
            
            for(int j = 0; j < MAX_PARTICLES; ++j){
                float distance = sqrt((Particles[i].pos.x - Particles[j].pos.x)*
                                      (Particles[i].pos.x - Particles[j].pos.x)+
                                      (Particles[i].pos.y - Particles[j].pos.y)*
                                      (Particles[i].pos.y - Particles[j].pos.y)+
                                      (Particles[i].pos.z - Particles[j].pos.z)*
                                      (Particles[i].pos.z - Particles[j].pos.z)
                                      )/18;
                force += glm::vec3((Particles[j].pos.x - Particles[i].pos.x),
                                   (Particles[j].pos.y - Particles[i].pos.y),
                                   (Particles[j].pos.z - Particles[i].pos.z))*distance*(deltaTime/100000);
            }
            
            Particles[i].vel += force;
            Particles[i].pos += Particles[i].vel;
            data[3*i] = Particles[i].pos.x;      // Modify X values
            data[3*i+1] = Particles[i].pos.y;    // Modify Y values
            data[3*i+2] = Particles[i].pos.z;    // Modify Z values
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        
    } else {
        printf ("Could not change buffer data");
    }
    
}

