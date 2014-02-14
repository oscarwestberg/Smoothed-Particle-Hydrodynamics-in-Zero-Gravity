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
const float TIME_STEP = 0.005;
const float DISTANCE = 0.03;
const float PARTICLE_MASS = 0.05;
const float IDEAL_DENSITY = 1000;
const float GRAVITY = -9.82;
const float VISCOSITY_CONST = 20;
const float SURFACE_TENSION = 0.003;
const float TENSION_THRESHOLD = 12;
const float STIFFNESS = 4;
const float H = 0.09;

const float SMOOTHING_KERNAL_CONST = PARTICLE_MASS*1.5675/std::powf(H,9);
const float SMOOTHING_GRADIENT_CONST = PARTICLE_MASS*14.3312/std::powf(H,6);


void ParticleSystem::initParticleSystem()
{
    // Give all the particles an initial value
    //for(int i = 0; i < MAX_PARTICLES; i++){
    //    Particles[i].pos.x = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
    //    Particles[i].pos.y = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
    //    Particles[i].pos.z = static_cast <float> (rand() % 1000) / static_cast <float> (500) -1;
    //}

	int i = 0;
	for(int xPos = 0; xPos < SIZE; xPos++){
		for(int yPos = 0; yPos < SIZE; yPos++){
			Particles[i].pos.x = xPos*DISTANCE - DISTANCE*SIZE/2;
			Particles[i].pos.y = yPos*DISTANCE - DISTANCE*SIZE/2;
			i++;
		} 
	}
}

void ParticleSystem::updateParticles(float deltaTime)
{
	updateDensity();
	updatePressure();
	glm::vec3 accelerationVec;
	glm::vec3 presGradVec;
	glm::vec3 viscVec;

	// for debugging
	viscVec = calcViscosity(Particles[1]);
	float coordX = viscVec.x;
	float coordY = viscVec.y;

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

		// Save particle positions in seperate variable
        positions[i] = Particles[i].pos;

		presGradVec = calcPressureGradient(Particles[i]);
		viscVec = calcViscosity(Particles[i]);
		accelerationVec = (-presGradVec) + viscVec;
        Particles[i].vel = Particles[i].vel + TIME_STEP*accelerationVec;
        Particles[i].pos = Particles[i].pos + (TIME_STEP*Particles[i].vel);
    }
}

void ParticleSystem::updateDensity(){
	for(int j = 0; j < MAX_PARTICLES; j++){
		float temp_density_sum = 0;
		for(int i = 0; i < MAX_PARTICLES; i++){
			glm::vec3 temp_dist = Particles[j].pos - Particles[i].pos;
			if(glm::length(temp_dist)<H)
			{
				float dist = glm::length(temp_dist);
				float powertre = std::powf((std::powf(H,2) - std::powf(dist,2)),3);
				float blaha = SMOOTHING_KERNAL_CONST*powertre;
				temp_density_sum += blaha;
			}
		}
		Particles[j].density = temp_density_sum;
	}
}

void ParticleSystem::updatePressure(){
	for(int i = 0; i < MAX_PARTICLES; i++){
		Particles[i].pressure = STIFFNESS*(Particles[i].density - IDEAL_DENSITY);
	}
}

glm::vec3 ParticleSystem::calcPressureGradient(Particle &particle){
	glm::vec3 temp_pressure_vec = glm::vec3(0);
	glm::vec3 temp_dist;
	for(int i = 0; i < MAX_PARTICLES; i++){
		temp_dist = particle.pos - Particles[i].pos;
		if(glm::length(temp_dist) < H && glm::length(temp_dist) != 0){
			float pressureMag = ((particle.pressure/std::powf(particle.density,2))+(Particles[i].pressure/std::powf(Particles[i].density,2)))*std::powf((H - glm::length(temp_dist)),2);
			temp_pressure_vec += pressureMag*glm::normalize(temp_dist);
		}
	}
	temp_pressure_vec *= (-SMOOTHING_GRADIENT_CONST);
	return temp_pressure_vec;
}

glm::vec3 ParticleSystem::calcViscosity(Particle &particle){
	glm::vec3 temp_viscosity_vec = glm::vec3(0);
	float temp_dist_abs;
	for(int i = 0; i < MAX_PARTICLES; i++){
		temp_dist_abs = glm::length(particle.pos - Particles[i].pos);
		//std::cout << temp_dist_abs << std::endl;
		if(temp_dist_abs < H){
			//std::cout << "velocity.x: " << (Particles[i].vel - particle.vel).x << ", velocity.y: " << (Particles[i].vel - particle.vel).y << std::endl;
			temp_viscosity_vec += ((Particles[i].vel - particle.vel)/Particles[i].density)*(H - temp_dist_abs);
		}
	}
	temp_viscosity_vec = ((VISCOSITY_CONST*SMOOTHING_GRADIENT_CONST)/particle.density)*temp_viscosity_vec;
	return temp_viscosity_vec;
}




