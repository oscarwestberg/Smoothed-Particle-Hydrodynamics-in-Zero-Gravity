
#ifndef __Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Particles__
#define __Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Particles__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <map>

//#define MAX_PARTICLES 100

class VoxelGrid
{   
	const int voxel_width = 0.07*2;
	struct Particle;
	public:
		void initVoxelGrid(const std::vector<Particle> &Particles, float H);		
		std::map<int, std::vector<Particle>> Voxel;

	private:

};

#endif /* defined(__Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Particles__) */
