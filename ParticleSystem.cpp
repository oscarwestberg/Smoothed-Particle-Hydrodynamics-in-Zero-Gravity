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
const float STIFFNESS = 4;
const float H = 0.07;
const bool ZEROGRAVITY = true;

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
	
	// BOX_SIZE*2.1 => cellGrid will be slightly outside of boundingbox
	voxelGrid.Setup(BOX_SIZE*2.4, BOX_SIZE*2.4, H);
}

void ParticleSystem::updateParticles(float deltaTime)
{
	for(int i = 0; i < MAX_PARTICLES; i++){
		voxelGrid.RegisterObject(Particles[i], i);
	}	
	
	updateDensityWithBuckets();
	updatePressure();
	updatePressureGradientWithBuckets();
	updateViscosityWithBuckets();
	
	//updateDensity();
	//updatePressure();	
	//updatePressureGradient();
	//updateViscosity();
	
	glm::vec3 accelerationVec;

	//std::cout << "UPDATE TIME STEP" << std::endl;
    for(int i = 0; i < MAX_PARTICLES; i++ ){
		// Save particle positions in seperate variable
		//std::cout << "Pre pos.x: " << Particles[i].pos.x << std::endl << "Pre pos.y: " << Particles[i].pos.y<< std::endl;
		//std::cout << "Pre vel.x: " << Particles[i].vel.x << std::endl << "Pre vel.y: " << Particles[i].vel.y<< std::endl;
		//std::cout << i << " Pre visc.x: " << Particles[i].viscosity.x << "   ";
        positions[i] = Particles[i].pos;
		accelerationVec = (-Particles[i].pressureGradient) + Particles[i].viscosity;
		if(!ZEROGRAVITY){
			accelerationVec += glm::vec3(0,-9.8, 0);
		}
        Particles[i].vel = Particles[i].vel + TIME_STEP*accelerationVec;
        Particles[i].pos = Particles[i].pos + (TIME_STEP*Particles[i].vel);
		//std::cout << "After pos.x: " << Particles[i].pos.x << std::endl << "After pos.y: " << Particles[i].pos.y << " i: " << i << std::endl;
		//std::cout << "After vel.x: " << Particles[i].vel.x << std::endl << "After vel.y: " << Particles[i].vel.y<< std::endl;
		//std::cout << "After visc.x: " << Particles[i].viscosity.x << std::endl;
    }

	voxelGrid.ClearBuckets();
	updateBoundingConditions();
}

void ParticleSystem::mouseInput(float x, float y, int width, int height){
    x = ((x/width)*2)-1;
    y = ((y/height)*2)-1;
    
    for(int j = 0; j < MAX_PARTICLES; j++) {
			//std::cout << "x: " << x << " y: " << y << " part.x: " << Particles[j].pos.x << " part.y: " << Particles[j].pos.y << std::endl;
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
	//std::cout << "UPDATE DENSITY WITH BUCKETS: " << std::endl;
	for(int j = 0; j < MAX_PARTICLES; j++) {
		//std::cout << j << " INNAN density: " << Particles[j].density << std::endl;
		float temp_density_sum = 0;
		int *listOfParticleIdsPointer = voxelGrid.GetNearby(Particles[j]); 
		for(int it = 0; it < voxelGrid.kernelParticles; it++) {
			if(*(listOfParticleIdsPointer+it) >= 0){
				glm::vec3 temp_dist = Particles[j].pos - Particles[*(listOfParticleIdsPointer+it)].pos;
				float dist = glm::length(temp_dist);
				if(dist<H){
					temp_density_sum += SMOOTHING_KERNAL_CONST*std::powf((std::powf(H,2) - std::powf(dist,2)),3);
				}
			}
			else
			{
				break;
			}
		}

		if(temp_density_sum != 0)
			Particles[j].density = temp_density_sum;

		//std::cout << "EFTER density: " << Particles[j].density << std::endl;
	}
}

void ParticleSystem::updatePressure(){
	//std::cout << "UPDATE PRESSURE" << std::endl;
	for(int i = 0; i < MAX_PARTICLES; i++){
		//std::cout << "Pre pressure: " << Particles[i].pressure << std::endl;
		Particles[i].pressure = STIFFNESS*(Particles[i].density - IDEAL_DENSITY);
		//std::cout << "After pressure: " << Particles[i].pressure << std::endl;
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
	//std::cout << "UPDATE PRESSUER GRADIENT" << std::endl;
	for(int j = 0; j < MAX_PARTICLES; j++){
		//std::cout << "Pre pressure gradient.x: " << Particles[j].pressureGradient.x << "pressure gradient.y: " << Particles[j].pressureGradient.y<< std::endl;
		glm::vec3 temp_pressure_vec = glm::vec3(0);
		glm::vec3 temp_dist;
		int* listOfParticleIdsPointer = voxelGrid.GetNearby(Particles[j]); 
		for(int it = 0; it < voxelGrid.kernelParticles; it++) {
			if(*(listOfParticleIdsPointer+it) >= 0){
				temp_dist = Particles[j].pos - Particles[*(listOfParticleIdsPointer+it)].pos;
				if(glm::length(temp_dist) < H && glm::length(temp_dist) != 0){
					float pressureMag = ((Particles[j].pressure/std::powf(Particles[j].density,2))+(Particles[*(listOfParticleIdsPointer+it)].pressure/std::powf(Particles[*(listOfParticleIdsPointer+it)].density,2)))*std::powf((H - glm::length(temp_dist)),2);
					temp_pressure_vec += pressureMag*glm::normalize(temp_dist);
				}
			}
			else
			{
				break;
			}
		}
		//std::cout << "After pressure gradient.x: " << Particles[j].pressureGradient.x << "pressure gradient.y: " << Particles[j].pressureGradient.y<< std::endl;
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

		int *listOfParticleIdsPointer = voxelGrid.GetNearby(Particles[j]); 
		for(int it = 0; it < voxelGrid.kernelParticles; it++) {
			if(*(listOfParticleIdsPointer+it) >= 0){
				temp_dist_abs = glm::length(Particles[j].pos - Particles[*(listOfParticleIdsPointer+it)].pos);
				if(temp_dist_abs < H){
					temp_viscosity_vec += ((Particles[*(listOfParticleIdsPointer+it)].vel - Particles[j].vel)/Particles[*(listOfParticleIdsPointer+it)].density)*(H - temp_dist_abs);
				}
			}
			else
			{
				break;
			}
		}

		temp_viscosity_vec = ((VISCOSITY_CONST*SMOOTHING_GRADIENT_CONST)/Particles[j].density)*temp_viscosity_vec;
		Particles[j].viscosity = temp_viscosity_vec;
	}
}

void ParticleSystem::updateBoundingConditions(){
    float energyLoss = 1;
    
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



