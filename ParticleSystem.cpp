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
const float BOX_SIZE = 0.4;



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
		
	voxelGrid.Setup(BOX_SIZE*2, BOX_SIZE*2, H);
}

void ParticleSystem::updateParticles(float deltaTime)
{
	for(int i = 0; i < MAX_PARTICLES; i++){
		voxelGrid.RegisterObject(Particles[i], i);

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

void ParticleSystem::mouseInput(float x, float y, int width, int height){
    x = ((x/width)*2)-1;
    y = ((y/height)*2)-1;
    
    for(int j = 0; j < MAX_PARTICLES; j++) {
        float distance = glm::distance(glm::vec3(x,y,0),glm::vec3(Particles[j].pos.x,Particles[j].pos.y, 0));
        float xForce = glm::length(Particles[j].pos.x + x);
        float yForce = glm::length(Particles[j].pos.y + y);
        
        if(x >= Particles[j].pos.x)
            Particles[j].vel.x -= (xForce / distance)*0.09;
        else
            Particles[j].vel.x += (xForce / distance)*0.09;
        if(y >= Particles[j].pos.y)
            Particles[j].vel.y += (yForce / distance)*0.09;
        else
            Particles[j].vel.y -= (yForce / distance)*0.09;
    }
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
		std::vector<int> listOfParticleIds = voxelGrid.GetNearby(Particles[j]); 
		for(std::vector<int>::iterator it = listOfParticleIds.begin(); it != listOfParticleIds.end(); it++) {
			glm::vec3 temp_dist = Particles[j].pos - Particles[*it].pos;
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

		std::vector<int> listOfParticleIds = voxelGrid.GetNearby(Particles[j]); 
		for(std::vector<int>::iterator it = listOfParticleIds.begin(); it != listOfParticleIds.end(); it++) {
			temp_dist = Particles[j].pos - Particles[*it].pos;
			if(glm::length(temp_dist)<H){
				float pressureMag = ((Particles[j].pressure/std::powf(Particles[j].density,2))+(Particles[*it].pressure/std::powf(Particles[*it].density,2)))*std::powf((H - glm::length(temp_dist)),2);
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

		std::vector<int> listOfParticleIds = voxelGrid.GetNearby(Particles[j]); 
		for(std::vector<int>::iterator it = listOfParticleIds.begin(); it != listOfParticleIds.end(); it++) {
			temp_dist_abs = glm::length(Particles[j].pos - Particles[*it].pos);
			//std::cout << temp_dist_abs << std::endl;
			if(temp_dist_abs < H){
				//std::cout << "velocity.x: " << (Particles[i].vel - particle.vel).x << ", velocity.y: " << (Particles[i].vel - particle.vel).y << std::endl;
				temp_viscosity_vec += ((Particles[*it].vel - Particles[j].vel)/Particles[*it].density)*(H - temp_dist_abs);
			}
		}

		temp_viscosity_vec = ((VISCOSITY_CONST*SMOOTHING_GRADIENT_CONST)/Particles[j].density)*temp_viscosity_vec;
		Particles[j].viscosity = temp_viscosity_vec;
	}
}

void ParticleSystem::updateBoundingConditions(){
    float energyLoss = 0.8;
    
	for(int i = 0; i < MAX_PARTICLES; i++){
		if(Particles[i].pos.x < -BOX_SIZE){
			Particles[i].pos.x = -BOX_SIZE;
            Particles[i].vel.x = -Particles[i].vel.x*energyLoss;
		}
		else if(Particles[i].pos.x > BOX_SIZE){
			Particles[i].pos.x = BOX_SIZE;
            Particles[i].vel.x = -Particles[i].vel.x*energyLoss;
		}
		if(Particles[i].pos.y < -BOX_SIZE){
			Particles[i].pos.y = -BOX_SIZE;
            Particles[i].vel.y = -Particles[i].vel.y*energyLoss;
		}
		else if(Particles[i].pos.y > BOX_SIZE){
			Particles[i].pos.y = BOX_SIZE;
            Particles[i].vel.y = -Particles[i].vel.y*energyLoss;
		}
	}
}



