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

// Constant parameters
const float TIME_STEP = 0.01;
const float DISTANCE = 0.03;
const float PARTICLE_MASS = 0.05;
const float IDEAL_DENSITY = 1000;
const float GRAVITY = -9.82;
const float VISCOSITY_CONST = 10;
const float SURFACE_TENSION = 0.003;
const float TENSION_THRESHOLD = 8;
const float STIFFNESS = 5;
const float H = 0.07;


void ParticleSystem::initParticleSystem()
{
    // Give all the particles an initial value
    for(int i = 0; i < MAX_PARTICLES; i++){
        Particles[i].pos.x = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
        Particles[i].pos.y = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
        Particles[i].pos.z = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
    }
}

void ParticleSystem::updateParticles(float deltaTime)
{
	//updateDensity();

    for(int i = 0; i < MAX_PARTICLES; ++i ){

        glm::vec3 force;
        
        // Very ugly way to update the particles
        //for(int j = 0; j < MAX_PARTICLES; ++j){
        //    float distance = sqrt((Particles[i].pos.x - Particles[j].pos.x)*
        //                          (Particles[i].pos.x - Particles[j].pos.x)+
        //                          (Particles[i].pos.y - Particles[j].pos.y)*
        //                          (Particles[i].pos.y - Particles[j].pos.y)+
        //                          (Particles[i].pos.z - Particles[j].pos.z)*
        //                          (Particles[i].pos.z - Particles[j].pos.z)
        //                          )/18;
        //    force += glm::vec3((Particles[j].pos.x - Particles[i].pos.x),
        //                       (Particles[j].pos.y - Particles[i].pos.y),
        //                       (Particles[j].pos.z - Particles[i].pos.z))*distance*(deltaTime/1000);
        //}

		//calcPressureGradient();
		//calcViscosity();

        // Save particle positions in seperate variable
        positions[i] = Particles[i].pos;
        
        Particles[i].vel += force;
        Particles[i].pos += Particles[i].vel;
    }
}

void ParticleSystem::updateDensity(){
	for(int j = 0; j < MAX_PARTICLES; j++)
	{
		float temp_density_sum = 0;
		for(int i = 0; i < MAX_PARTICLES; i++){
			//CALC STUFF
			temp_density_sum += 0;
		}
		Particles[j].density = PARTICLE_MASS*temp_density_sum;
	}
}

float ParticleSystem::calcPressureGradient(Particle &particle){
	for(int i = 0; i < MAX_PARTICLES; i++){
		//glm::vec3 temp_distance = glm::distance(particle.pos, Particles[i].pos);
	}
	return 0;
}

float ParticleSystem::calcViscosity(Particle &particle){
	return 0;
}




