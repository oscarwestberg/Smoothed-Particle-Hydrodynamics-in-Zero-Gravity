#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <map>
#include "Particle.h"

//#define MAX_PARTICLES 100
class VoxelGrid
{   
	public:
		
		float VOXEL_WIDTH;
		
		friend void renderVoxels();

		void initVoxelGrid(const std::vector<Particle> &Particles, float H);		
		int getHash(const Particle& particle, float voxelwidth){
			int x = floor(particle.pos.x/voxelwidth);
			int y = floor(particle.pos.y/voxelwidth);
			int z = floor(particle.pos.z/voxelwidth);
			// z * width * height + y * width + x
		
			int rx = x;
			int ry = y << 10;
			int rz = z << 20;
	
			int hashed = rx + ry + rz;

			return hashed;
		};
		
		std::map<int, std::vector<Particle>> Voxel;
		std::map<int, int> indexCounter;
		
	private:

};