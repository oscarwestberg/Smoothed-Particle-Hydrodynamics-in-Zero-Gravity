#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "Particle.h"

//#define MAX_PARTICLES 100

class VoxelGrid
{   
	const int VOXEL_WIDTH = 0.07*2;
	public:
		void initVoxelGrid(const std::vector<Particle> &Particles, float H);		
		std::map<int, std::vector<Particle>> Voxel;

	private:

};