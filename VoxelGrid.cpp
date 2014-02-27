
#include "VoxelGrid.h"
#include <vector>
#include <map>
#include <cmath>
/*
void VoxelGrid::initVoxelGrid(const std::vector<Particle> &Particles, float H) {
	// Every voxel is 2H*2H*2H
	// Create a ID for every voxel 
	
	// Hasha skiten 
	// Om voxeln inte finns så gör en ny
	// Ta in positions dela med storleken på voxeln, (2*H)
	int voxelIndex;
	int particlesUsed = 0;

	for(int i = 0; i < Particles.size(); i++) {
		voxelIndex = getHash(Particles[i]);
		Voxel[voxelIndex].push_back(Particles[i]);
		indexCounter[voxelIndex] = indexCounter[voxelIndex] + 1;
	}


	std::cout << "hej" << std::endl;
}*/


/*void clearVoxelGrid(){}*/

std::list<int> VoxelGrid::AddBucket(glm::vec2 vector,float w,std::list<int> buckettoaddto)
{  
	//double cellSize = 0.07;
	//float width = 2;

	// Här måste det fixaS! Dela med cellsize ist för 0.07
    int cellPosition = (int)( (floor(vector.x / 0.07)) + (floor(vector.y / 0.07)) * w );

	std::list<int>::iterator it = std::find(buckettoaddto.begin(), buckettoaddto.end(), cellPosition);

	if(it == buckettoaddto.end()){
		buckettoaddto.insert(buckettoaddto.end(), cellPosition);
	}
	
	return buckettoaddto;
}

void VoxelGrid::Setup(float scenewidth, float sceneheight, float particleH) {
	H = particleH;
	//columns = (int)ceil(scenewidth / (H*2));
   // rows = (int)ceil(sceneheight / (H*2));
	sceneWidth = scenewidth;
    sceneHeight = sceneheight;
	cellSize = sceneHeight/rows;

	for(int i = 0; i < rows*columns; i++){
		particleCounter[i] = 0;
	}

	for(int buckets = 0; buckets < rows*columns; buckets++)
		for(int particleIds = 0; particleIds < 32; particleIds++)
			Buckets[buckets][particleIds] = -1;
}

void VoxelGrid::RegisterObject(const Particle& particle, int particleId)
{
		int cellId = (int)(floor((particle.pos.x + (sceneWidth/2))/ (cellSize)) + (floor((particle.pos.y + (sceneHeight/2))/ (cellSize) ) * columns )); 
		
		if(particleCounter[cellId] < 50){
			Buckets[cellId][particleCounter[cellId]] = particleId;
			particleCounter[cellId]++;
		}

}

void VoxelGrid::ClearBuckets() {

	for(int i = 0; i < rows*columns; i++){
		particleCounter[i] = 0;
	}

	for(int buckets = 0; buckets < rows*columns; buckets++)
		for(int particleIds = 0; particleIds < 32; particleIds++)
			Buckets[buckets][particleIds] = -1;
}

std::vector<int> VoxelGrid::GetNearby(const Particle& particle) {
	
	int bucketIds[4];

	bucketIds[0] = (int)( (floor((particle.pos.x + cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y + cellSize/2 + sceneWidth/2) / (cellSize))) * columns );
	bucketIds[1] = (int)( (floor((particle.pos.x + cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y - cellSize/2 + sceneWidth/2) / (cellSize))) * columns );
	bucketIds[2] = (int)( (floor((particle.pos.x - cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y - cellSize/2 + sceneWidth/2) / (cellSize))) * columns );
	bucketIds[3] = (int)( (floor((particle.pos.x - cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y + cellSize/2 + sceneWidth/2) / (cellSize))) * columns );

	std::vector<int> particleIds;

	for(int i = 0; i < 4; i++){
		if(bucketIds[i] >= 0 && bucketIds[i] < rows*columns){
			for(int j = 0; j < particleCounter[bucketIds[i]]; j++){
				particleIds.push_back(Buckets[bucketIds[i]][j]);
			}
		}
		//objects.insert(objects.end(), Buckets[bucketIds[i]].begin(), Buckets[bucketIds[i]].end());
	}

	return particleIds;   
}