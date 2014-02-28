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
#include <vector>
#include "VoxelGrid.h"

#define SIZE 10
#define MAX_PARTICLES SIZE*SIZE
// Should also change MAX_PARTICLES in the fragment shader

class ParticleSystem
{   
public:
	std::vector<Particle> Particles;
	VoxelGrid voxelGrid;

    void initParticleSystem();
    void updateParticles(float DeltaTimeMillis);
    void mouseInput(float x, float y, int width, int height);

private:
	void updateDensityWithBuckets();
	void updatePressureWithBuckets();
	void updatePressureGradientWithBuckets();
	void updateViscosityWithBuckets();
	
	void updateDensity();
	void updatePressure();
	void updatePressureGradient();
	void updateViscosity();
	void updateBoundingConditions();
	
};

#endif /* defined(__Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Particles__) */
