//
//  ParticleSystem.cpp
//  Smoothed-Particle-Hydrodynamics-in-Zero-Gravity
//
//  Created by Oscar Westberg on 2014-01-29.
//  Copyright (c) 2014 Group 7. All rights reserved.
//

#include "ParticleSystem.h"

#define MAX_PARTICLES 300
#define MAX_VERTICES MAX_PARTICLES*3
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

struct Particle
{
    glm::vec3 pos;
    glm::vec3 vel;
    float LifetimeMillis; 
};

ParticleSystem::ParticleSystem(){

}

ParticleSystem::~ParticleSystem(){
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void ParticleSystem::initParticleSystem()
{
    Particle Particles[MAX_PARTICLES];
    GLfloat points[MAX_VERTICES];
    
    //srand(time(NULL)); // Not needed?
    
    for(int i = 1; i < MAX_PARTICLES+1; i++){
        Particles[i-1].pos.x = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
        Particles[i-1].pos.y = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
        Particles[i-1].pos.z = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
        
        points[i*3-2] = Particles[i-1].pos.x;
        points[i*3-1] = Particles[i-1].pos.y;
        points[i*3] = Particles[i-1].pos.z;
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

int ParticleSystem::getParticleAmount(){
    return MAX_VERTICES;
}

void ParticleSystem::updateParticles(int DeltaTimeMillis){

}

