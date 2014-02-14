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

#define SIZE 12
#define MAX_PARTICLES SIZE*SIZE

struct Particle
{
    glm::vec3 pos;
    glm::vec3 vel;
	glm::vec3 pressureGradient;
	glm::vec3 viscosity;
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
	void updateBoundingConditions();
	void updatePressureGradient();
	void updateViscosity();
};

#endif /* defined(__Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Particles__) */
