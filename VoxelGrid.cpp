
#include "VoxelGrid.h"
#include <vector>
#include <map>

int getHash(const Particle& particle , float voxelwidth) {
	// http://stackoverflow.com/questions/20286179/voxel-unique-id-in-3d-space
		
	// Fick feeling, nu kör vi!
	int x = ceil(particle.pos.x/voxelwidth);
	int y = ceil(particle.pos.y/voxelwidth);
	int z = ceil(particle.pos.z/voxelwidth);
	// z * width * height + y * width + x
		
	int rx = x;
	int ry = y << 10;
	int rz = z << 20;
	
	int hashed = rx + ry + rz;

	return hashed;
	//return glm::vec3(floor(position.x/voxel_width), floor(position.y/voxel_width), floor(position.z/voxel_width));
}

void VoxelGrid::initVoxelGrid(const std::vector<Particle> &Particles, float H) {
	VOXEL_WIDTH = 0.07*2.0;
	// Every voxel is 2H*2H*2H
	// Create a ID for every voxel 
	
	// Hasha skiten 
	// Om voxeln inte finns så gör en ny
	// Ta in positions dela med storleken på voxeln, (2*H)
	int voxelIndex;
	int particlesUsed = 0;

	for(int i = 0; i < Particles.size(); i++) {
		voxelIndex = getHash(Particles[i], VOXEL_WIDTH);
		Voxel[voxelIndex].push_back(Particles[i]);
		indexCounter[voxelIndex] = indexCounter[voxelIndex] + 1;
	}


	std::cout << "hej" << std::endl;
}
/*
void changeVoxel() {
	
	int voxelIndex = getHash(position, voxel_width);
	addParticleInVoxel(voxelIndex);
	removeParticleInVoxel(voxelIndex);

}

// Add a particle to a voxel
void addParticleInVoxel(int voxelIndex) {
	Voxel[voxelIndex].push_back(particle);  
}

// Remove a particle from a voxel
void removeParticleInVoxel(int voxelIndex) {
	std::map<int, std::vector<glm::Particles>>::iterator tempVoxel;

	tempVoxel = Voxel.find(voxelIndex);
	if (tempVoxel != Voxel.end()) {
		vector<Particles> tempVec = Voxel[voxelIndex];
		
		item = particle;
		
		tempVec.erase(std::find(vector.begin(), vector.end(), item)!=vector.end());

		Voxel[voxelIndex] = tempVec;
	}
}*/