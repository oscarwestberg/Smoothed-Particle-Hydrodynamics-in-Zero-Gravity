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
const float VISCOSITY_CONST = 5;
const float SURFACE_TENSION = 0.003;
const float TENSION_THRESHOLD = 12;
const float STIFFNESS = 3.5;
const float H = 0.07;
const bool ZEROGRAVITY = false;

// Pre calculated stuff
const float SMOOTHING_KERNAL_CONST = PARTICLE_MASS*1.5675/std::powf(H,9);
const float SMOOTHING_GRADIENT_CONST = PARTICLE_MASS*14.3312/std::powf(H,6);

// Bounding box
const float BOX_SIZE = 0.2;



VoxelGrid voxelGrid;

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
			Particle tempParticle;
			tempParticle.pos.x = xPos*DISTANCE - DISTANCE*SIZE/2;
			tempParticle.pos.y = yPos*DISTANCE - DISTANCE*SIZE/2;
			Particles.push_back(tempParticle);
			i++;
		} 
	}
		
	voxelGrid.Setup(0.6, 0.6, H);
}

void ParticleSystem::updateParticles(float deltaTime)
{
	for(int i = 0; i < MAX_PARTICLES; i++){
		voxelGrid.RegisterObject(Particles[i]);

	}	
	
	updateDensityWithBuckets();
	//updatePressureWithBuckets();
	//updatePressureGradientWithBuckets();
	//updateViscosityWithBuckets();
	
	//updateDensity();
	updatePressure();	
	updatePressureGradient();
	updateViscosity();
	//updateDensity();
	//updatePressure();
	// updatePressureGradient();
	
	glm::vec3 accelerationVec;

    for(int i = 0; i < MAX_PARTICLES; ++i ){
		// Save particle positions in seperate variable
        positions[i] = Particles[i].pos;
		accelerationVec = (-Particles[i].pressureGradient) + Particles[i].viscosity;
		if(!ZEROGRAVITY){
			accelerationVec += glm::vec3(0,-9.8, 0);
		}
        Particles[i].vel = Particles[i].vel + TIME_STEP*accelerationVec;
        Particles[i].pos = Particles[i].pos + (TIME_STEP*Particles[i].vel);
    }

	voxelGrid.ClearBuckets();
	updateBoundingConditions();
}

void ParticleSystem::updateDensity() {
	for(int j = 0; j < MAX_PARTICLES; j++) {
		float temp_density_sum = 0;
		for(int i = 0; i < MAX_PARTICLES; i++) {
			glm::vec3 temp_dist = Particles[j].pos - Particles[i].pos;
			if(glm::length(temp_dist)<H){
				float dist = glm::length(temp_dist);
				temp_density_sum += SMOOTHING_KERNAL_CONST*std::powf((std::powf(H,2) - std::powf(dist,2)),3);
			}
		}
		Particles[j].density = temp_density_sum;
	}
}

void ParticleSystem::updateDensityWithBuckets() {
	for(int j = 0; j < MAX_PARTICLES; j++) {
		float temp_density_sum = 0;
		std::list<Particle> listofparticles = voxelGrid.GetNearby(Particles[j]); 
		for(std::list<Particle>::iterator it = listofparticles.begin(); it != listofparticles.end(); it++) {
			glm::vec3 temp_dist = Particles[j].pos - (*it).pos;
			if(glm::length(temp_dist)<H){
				float dist = glm::length(temp_dist);
				temp_density_sum += SMOOTHING_KERNAL_CONST*std::powf((std::powf(H,2) - std::powf(dist,2)),3);
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

void ParticleSystem::updatePressureGradient(){
	for(int j = 0; j < MAX_PARTICLES; j++){
		glm::vec3 temp_pressure_vec = glm::vec3(0);
		glm::vec3 temp_dist;
		for(int i = 0; i < MAX_PARTICLES; i++){
			temp_dist = Particles[j].pos - Particles[i].pos;
			if(glm::length(temp_dist) < H && glm::length(temp_dist) != 0){
				float pressureMag = ((Particles[j].pressure/std::powf(Particles[j].density,2))+(Particles[i].pressure/std::powf(Particles[i].density,2)))*std::powf((H - glm::length(temp_dist)),2);
				temp_pressure_vec += pressureMag*glm::normalize(temp_dist);
			}
		}
		temp_pressure_vec *= (-SMOOTHING_GRADIENT_CONST);
		Particles[j].pressureGradient = temp_pressure_vec;
	}

}

void ParticleSystem::updatePressureGradientWithBuckets(){
	for(int j = 0; j < MAX_PARTICLES; j++){
		glm::vec3 temp_pressure_vec = glm::vec3(0);
		glm::vec3 temp_dist;

		std::list<Particle> listofparticles = voxelGrid.GetNearby(Particles[j]); 
		for(std::list<Particle>::iterator it = listofparticles.begin(); it != listofparticles.end(); it++) {
			temp_dist = Particles[j].pos - (*it).pos;
			if(glm::length(temp_dist)<H){
				float pressureMag = ((Particles[j].pressure/std::powf(Particles[j].density,2))+((*it).pressure/std::powf((*it).density,2)))*std::powf((H - glm::length(temp_dist)),2);
				glm::vec3 normalizedTempDist= glm::normalize(temp_dist);
				temp_pressure_vec += pressureMag*normalizedTempDist;
			}
		}
		temp_pressure_vec *= (-SMOOTHING_GRADIENT_CONST);
		Particles[j].pressureGradient = temp_pressure_vec;
	}

}

void ParticleSystem::updateViscosity(){
	for(int j = 0; j < MAX_PARTICLES; j++){
		glm::vec3 temp_viscosity_vec = glm::vec3(0);
		float temp_dist_abs;
		for(int i = 0; i < MAX_PARTICLES; i++){
			temp_dist_abs = glm::length(Particles[j].pos - Particles[i].pos);
			//std::cout << temp_dist_abs << std::endl;
			if(temp_dist_abs < H){
				//std::cout << "velocity.x: " << (Particles[i].vel - particle.vel).x << ", velocity.y: " << (Particles[i].vel - particle.vel).y << std::endl;
				temp_viscosity_vec += ((Particles[i].vel - Particles[j].vel)/Particles[i].density)*(H - temp_dist_abs);
			}
		}
		temp_viscosity_vec = ((VISCOSITY_CONST*SMOOTHING_GRADIENT_CONST)/Particles[j].density)*temp_viscosity_vec;
		Particles[j].viscosity = temp_viscosity_vec;
	}
}

void ParticleSystem::updateViscosityWithBuckets(){
	for(int j = 0; j < MAX_PARTICLES; j++){
		glm::vec3 temp_viscosity_vec = glm::vec3(0);
		float temp_dist_abs;

		std::list<Particle> listofparticles = voxelGrid.GetNearby(Particles[j]); 
		for(std::list<Particle>::iterator it = listofparticles.begin(); it != listofparticles.end(); it++) {
			temp_dist_abs = glm::length(Particles[j].pos - (*it).pos);
			//std::cout << temp_dist_abs << std::endl;
			if(temp_dist_abs < H){
				//std::cout << "velocity.x: " << (Particles[i].vel - particle.vel).x << ", velocity.y: " << (Particles[i].vel - particle.vel).y << std::endl;
				temp_viscosity_vec += (((*it).vel - Particles[j].vel)/(*it).density)*(H - temp_dist_abs);
			}
		}

		temp_viscosity_vec = ((VISCOSITY_CONST*SMOOTHING_GRADIENT_CONST)/Particles[j].density)*temp_viscosity_vec;
		Particles[j].viscosity = temp_viscosity_vec;
	}
}

void ParticleSystem::updateBoundingConditions(){
	for(int i = 0; i < MAX_PARTICLES; i++){		
		bool outsideX = false;
		bool outsideY = false;
		glm::vec2 normalX;
		glm::vec2 normalY;
		float contactPointX;
		float contactPointY;

		if(Particles[i].pos.x < -BOX_SIZE){
			outsideX = true;
			normalX = glm::vec2(1,0);
			contactPointX = -BOX_SIZE;
		}
		else if(Particles[i].pos.x > BOX_SIZE){
			outsideX = true;
			normalX = glm::vec2(-1,0);
			contactPointX = BOX_SIZE;
		}
		if(Particles[i].pos.y < -BOX_SIZE){
			outsideY = true;
			normalY = glm::vec2(0,1);
			contactPointY = -BOX_SIZE;
		}
		else if(Particles[i].pos.y > BOX_SIZE){
			outsideY = true;
			normalY = glm::vec2(0,-1);
			contactPointY = BOX_SIZE;
		}
       
		if(outsideX){
			float penDepth = abs(contactPointX - Particles[i].pos.x);
			Particles[i].vel = Particles[i].vel - glm::vec3((1+1)*(glm::dot(glm::vec2(Particles[i].vel.x,Particles[i].vel.y),normalX))*normalX,0);
			Particles[i].pos = Particles[i].pos + glm::vec3(penDepth*normalX,0);
		}
		if(outsideY){
			float penDepth = abs(contactPointY - Particles[i].pos.y);
			Particles[i].vel = Particles[i].vel - glm::vec3((1+1)*(glm::dot(glm::vec2(Particles[i].vel.x,Particles[i].vel.y),normalY))*normalY,0);
			Particles[i].pos = Particles[i].pos + glm::vec3(penDepth*normalY,0);
		}
	}
}



