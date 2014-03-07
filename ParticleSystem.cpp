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

glm::vec2 positions[MAX_PARTICLES];

// Constant parameters
const float TIME_STEP = 0.005;
const float DISTANCE = 0.02;
const float PARTICLE_MASS = 0.02;
const float IDEAL_DENSITY = 1000;
const float GRAVITY = -9.82;
const float VISCOSITY_CONST = 5;
//const float SURFACE_TENSION = 0.003;
//const float TENSION_THRESHOLD = 12;
const float STIFFNESS = 5;
const float H = 0.05;
const bool ZEROGRAVITY = false;

// Pre calculated stuff
const float SMOOTHING_KERNAL_CONST = PARTICLE_MASS*1.5675/std::powf(H,9);
const float SMOOTHING_GRADIENT_CONST = PARTICLE_MASS*14.3312/std::powf(H,6);

// Bounding box
const float BOX_SIZE = 0.4;

VoxelGrid voxelGrid;

// Save ID of nearby particles
int listOfParticleIds[MAX_PARTICLES][voxelGrid.kernelParticles];

void ParticleSystem::initParticleSystem()
{
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
	// Update spatial grid
	for(int i = 0; i < MAX_PARTICLES; i++){
		voxelGrid.RegisterObject(Particles[i], i);
		// reset list of all nearby particles
		for(int j = 0; j < voxelGrid.kernelParticles; j++){
			listOfParticleIds[i][j] = -1;
		}
	}	
	
	updateNeighbouringParticles();
	updateDensityWithBuckets();
	updatePressure();
	updatePressureGradientAndViscosityWithBuckets();
	//updatePressureGradientWithBuckets();
	//updateViscosityWithBuckets();
	
	//updateDensity();
	//updatePressure();	
	//updatePressureGradient();
	//updateViscosity();
	
	glm::vec2 accelerationVec;

    for(int i = 0; i < MAX_PARTICLES; i++ ){
        positions[i] = Particles[i].pos;
		accelerationVec = (-Particles[i].pressureGradient) + Particles[i].viscosity;
		if(!ZEROGRAVITY){
			accelerationVec += glm::vec2(0,-9.8);
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
    float forceStrength = 0.5;

    for(int j = 0; j < MAX_PARTICLES; j++) {
			float distance = glm::distance(glm::vec2(x,y),glm::vec2(Particles[j].pos.x,Particles[j].pos.y));
			float xForce = glm::length(Particles[j].pos.x + x);
			float yForce = glm::length(Particles[j].pos.y + y);

			if(x >= Particles[j].pos.x)
				Particles[j].vel.x -= (xForce / distance)*forceStrength;
			else
				Particles[j].vel.x += (xForce / distance)*forceStrength;
			if(y >= Particles[j].pos.y)
				Particles[j].vel.y += (yForce / distance)*forceStrength;
			else
				Particles[j].vel.y -= (yForce / distance)*forceStrength;
    }
}

void ParticleSystem::updateNeighbouringParticles(){
	for(int j = 0; j < MAX_PARTICLES; j++) {
		int *listOfParticleIdsPointer = voxelGrid.GetNearby(Particles[j]);
		for(int it = 0; it < voxelGrid.kernelParticles; it++) {
			if(*(listOfParticleIdsPointer+it) >= 0){
				listOfParticleIds[j][it] = *(listOfParticleIdsPointer+it);
			}
		}
	}
}

//void ParticleSystem::updateDensity() {
//	for(int j = 0; j < MAX_PARTICLES; j++) {
//		float temp_density_sum = 0;
//		for(int i = 0; i < MAX_PARTICLES; i++) {
//			glm::vec2 temp_dist = Particles[j].pos - Particles[i].pos;
//			if(glm::length(temp_dist)<H){
//				float dist = glm::length(temp_dist);
//				temp_density_sum += SMOOTHING_KERNAL_CONST*std::powf((std::powf(H,2) - std::powf(dist,2)),3);
//			}
//		}
//		Particles[j].density = temp_density_sum;
//	}
//}

void ParticleSystem::updateDensityWithBuckets() {
	for(int j = 0; j < MAX_PARTICLES; j++) {
		float temp_density_sum = 0;
		for(int it = 0; it < voxelGrid.kernelParticles; it++) {
			if(listOfParticleIds[j][it] >= 0){
				glm::vec2 temp_dist = Particles[j].pos - Particles[listOfParticleIds[j][it]].pos;
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
	}
}

void ParticleSystem::updatePressure(){
	for(int i = 0; i < MAX_PARTICLES; i++){
		Particles[i].pressure = STIFFNESS*(Particles[i].density - IDEAL_DENSITY);
	}
}

//void ParticleSystem::updatePressureGradient(){
//	for(int j = 0; j < MAX_PARTICLES; j++){
//		glm::vec2 temp_pressure_vec = glm::vec2(0);
//		glm::vec2 temp_dist;
//		for(int i = 0; i < MAX_PARTICLES; i++){
//			temp_dist = Particles[j].pos - Particles[i].pos;
//			if(glm::length(temp_dist) < H && glm::length(temp_dist) != 0){
//				float pressureMag = ((Particles[j].pressure/std::powf(Particles[j].density,2))+(Particles[i].pressure/std::powf(Particles[i].density,2)))*std::powf((H - glm::length(temp_dist)),2);
//				temp_pressure_vec += pressureMag*glm::normalize(temp_dist);
//			}
//		}
//		temp_pressure_vec *= (-SMOOTHING_GRADIENT_CONST);
//		Particles[j].pressureGradient = temp_pressure_vec;
//	}
//
//}

//void ParticleSystem::updatePressureGradientWithBuckets(){
//	for(int j = 0; j < MAX_PARTICLES; j++){
//		glm::vec2 temp_pressure_vec = glm::vec2(0);
//		glm::vec2 temp_dist;
//		int* listOfParticleIdsPointer = voxelGrid.GetNearby(Particles[j]); 
//		for(int it = 0; it < voxelGrid.kernelParticles; it++) {
//			if(*(listOfParticleIdsPointer+it) >= 0){
//				temp_dist = Particles[j].pos - Particles[*(listOfParticleIdsPointer+it)].pos;
//				if(glm::length(temp_dist) < H && glm::length(temp_dist) != 0){
//					float pressureMag = ((Particles[j].pressure/std::powf(Particles[j].density,2))+(Particles[*(listOfParticleIdsPointer+it)].pressure/std::powf(Particles[*(listOfParticleIdsPointer+it)].density,2)))*std::powf((H - glm::length(temp_dist)),2);
//					temp_pressure_vec += pressureMag*glm::normalize(temp_dist);
//				}
//			}
//			else
//			{
//				break;
//			}
//		}
//		temp_pressure_vec *= (-SMOOTHING_GRADIENT_CONST);
//			Particles[j].pressureGradient = temp_pressure_vec;
//	}
//}
//
//void ParticleSystem::updateViscosity(){
//	for(int j = 0; j < MAX_PARTICLES; j++){
//		glm::vec2 temp_viscosity_vec = glm::vec2(0);
//		float temp_dist_abs;
//		for(int i = 0; i < MAX_PARTICLES; i++){
//			temp_dist_abs = glm::length(Particles[j].pos - Particles[i].pos);
//			if(temp_dist_abs < H){
//				temp_viscosity_vec += ((Particles[i].vel - Particles[j].vel)/Particles[i].density)*(H - temp_dist_abs);
//			}
//		}
//		temp_viscosity_vec = ((VISCOSITY_CONST*SMOOTHING_GRADIENT_CONST)/Particles[j].density)*temp_viscosity_vec;
//		Particles[j].viscosity = temp_viscosity_vec;
//	}
//}

//void ParticleSystem::updateViscosityWithBuckets(){
//	for(int j = 0; j < MAX_PARTICLES; j++){
//		glm::vec2 temp_viscosity_vec = glm::vec2(0);
//		float temp_dist_abs;
//
//		int *listOfParticleIdsPointer = voxelGrid.GetNearby(Particles[j]); 
//		for(int it = 0; it < voxelGrid.kernelParticles; it++) {
//			if(*(listOfParticleIdsPointer+it) >= 0){
//				temp_dist_abs = glm::length(Particles[j].pos - Particles[*(listOfParticleIdsPointer+it)].pos);
//				if(temp_dist_abs < H){
//					temp_viscosity_vec += ((Particles[*(listOfParticleIdsPointer+it)].vel - Particles[j].vel)/Particles[*(listOfParticleIdsPointer+it)].density)*(H - temp_dist_abs);
//				}
//			}
//			else
//			{
//				break;
//			}
//		}
//
//		temp_viscosity_vec = ((VISCOSITY_CONST*SMOOTHING_GRADIENT_CONST)/Particles[j].density)*temp_viscosity_vec;
//		Particles[j].viscosity = temp_viscosity_vec;
//	}
//}

void ParticleSystem::updatePressureGradientAndViscosityWithBuckets(){
	for(int j = 0; j < MAX_PARTICLES; j++){
		glm::vec2 temp_pressure_vec = glm::vec2(0);
		glm::vec2 temp_viscosity_vec = glm::vec2(0);
		glm::vec2 temp_dist;

		for(int it = 0; it < voxelGrid.kernelParticles; it++) {
			if(listOfParticleIds[j][it] >= 0){
				temp_dist = (Particles[j].pos - Particles[listOfParticleIds[j][it]].pos);
				if(glm::length(temp_dist) < H && glm::length(temp_dist) != 0){
					temp_viscosity_vec += ((Particles[listOfParticleIds[j][it]].vel - Particles[j].vel)/Particles[listOfParticleIds[j][it]].density)*(H - glm::length(temp_dist));
					float pressureMag = ((Particles[j].pressure/std::powf(Particles[j].density,2))+(Particles[listOfParticleIds[j][it]].pressure/std::powf(Particles[listOfParticleIds[j][it]].density,2)))*std::powf((H - glm::length(temp_dist)),2);
					temp_pressure_vec += pressureMag*glm::normalize(temp_dist);
				}
			}
			else
			{
				break;
			}
		}

		temp_viscosity_vec = ((VISCOSITY_CONST*SMOOTHING_GRADIENT_CONST)/Particles[j].density)*temp_viscosity_vec;
		Particles[j].viscosity = temp_viscosity_vec;

		temp_pressure_vec *= (-SMOOTHING_GRADIENT_CONST);
		Particles[j].pressureGradient = temp_pressure_vec;
	}
}

void ParticleSystem::updateBoundingConditions(){
    float energyLoss = 0.9;
    
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
