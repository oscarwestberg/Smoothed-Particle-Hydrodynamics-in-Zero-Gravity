//
//  ParticleSystem.h
//  Smoothed-Particle-Hydrodynamics-in-Zero-Gravity
//
//  Created by Oscar Westberg on 2014-01-29.
//  Copyright (c) 2014 Group 7. All rights reserved.
//

#ifndef __Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Particles__
#define __Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Particles__

#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class ParticleSystem
{
    
public:
    ParticleSystem(); //constructor
    ~ParticleSystem(); //destructor
    int getParticleAmount();
    void initParticleSystem();
    void updateParticles(float DeltaTimeMillis);
    
private:
    GLuint vao;
    GLuint vbo;
};

#endif /* defined(__Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Particles__) */
