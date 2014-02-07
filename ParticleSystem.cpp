//
//  ParticleSystem.cpp
//  Smoothed-Particle-Hydrodynamics-in-Zero-Gravity
//
//  Created by Oscar Westberg on 2014-01-29.
//  Copyright (c) 2014 Group 7. All rights reserved.
//

#include "ParticleSystem.h"
#include <math.h>

#define MAX_VERTICES MAX_PARTICLES*3

glm::vec3 positions[MAX_PARTICLES];


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
}

void ParticleSystem::updateParticles(float deltaTime)
{
    for(int i = 0; i < MAX_PARTICLES; ++i ){
        glm::vec3 force;
        
        // Very ugly way to update the particles
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
                               (Particles[j].pos.z - Particles[i].pos.z))*distance*(deltaTime/1000);
        }
        
        // Save particle positions in seperate variable
        positions[i] = Particles[i].pos;
        
        Particles[i].vel += force;
        Particles[i].pos += Particles[i].vel;
    }
}

