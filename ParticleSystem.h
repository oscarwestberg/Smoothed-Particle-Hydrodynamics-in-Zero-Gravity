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
#include <glm/glm.hpp>
#include <iostream>

#define SIZE 10
#define MAX_PARTICLES SIZE*SIZE

struct Particle
{
    glm::vec3 pos;
    glm::vec3 vel;
	float density;
	float pressure;
};

class ParticleSystem
{   
public:
    void initParticleSystem();
    void updateParticles(float DeltaTimeMillis);
    
    Particle Particles[MAX_PARTICLES];
    
private:
	void updateDensity();
	void updatePressure();
	glm::vec3 calcPressureGradient(Particle &particle);
	glm::vec3 calcViscosity(Particle &particle);
};

#endif /* defined(__Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Particles__) */
