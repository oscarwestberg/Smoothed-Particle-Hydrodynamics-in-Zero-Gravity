#include "VoxelGrid.h"
#include <vector>
#include <map>
#include <algorithm>    // std::random_shuffle
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
    //rows = (int)ceil(sceneheight / (H*2));
	std::cout << "rows and columns should be: " << (sceneheight / (H*2)) << std::endl;
	sceneWidth = scenewidth;
    sceneHeight = sceneheight;
	cellSize = sceneHeight/(rows-2);
	std::cout << "cellSize: " << cellSize << ", H: " << H << std::endl;

	for(int i = 0; i < rows*columns; i++){
		particleCounter[i] = 0;
	}

	for(int buckets = 0; buckets < rows*columns; buckets++)
		for(int particles = 0; particles < maxParticlesInCell; particles++)
			Buckets[buckets][particles] = -1;
}

void VoxelGrid::RegisterObject(const Particle& particle, int particleId)
{
		int cellId = (int)(floor((particle.pos.x + (sceneWidth/2))/ (cellSize)) + (floor((particle.pos.y + (sceneHeight/2))/ (cellSize) ) * columns )); 


		if(cellId >= 0 && cellId <= rows*columns){
				Buckets[cellId][particleCounter[cellId]] = particleId;
				particleCounter[cellId]++;
		}
		else
		{
			std::cout << "Particle outside of cellGrid. wtf to do?" << std::endl;
		}
}

void VoxelGrid::ClearBuckets() {

	for(int i = 0; i < rows*columns; i++){
		particleCounter[i] = 0;
	}

	for(int buckets = 0; buckets < rows*columns; buckets++)
		for(int particles = 0; particles < maxParticlesInCell; particles++)
			Buckets[buckets][particles] = -1;
}

int* VoxelGrid::GetNearby(const Particle& particle) {
	
	int bucketIds[4];

	bucketIds[0] = (int)( (floor((particle.pos.x + cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y + cellSize/2 + sceneWidth/2) / (cellSize))) * columns );
	bucketIds[1] = (int)( (floor((particle.pos.x + cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y - cellSize/2 + sceneWidth/2) / (cellSize))) * columns );
	bucketIds[2] = (int)( (floor((particle.pos.x - cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y - cellSize/2 + sceneWidth/2) / (cellSize))) * columns );
	bucketIds[3] = (int)( (floor((particle.pos.x - cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y + cellSize/2 + sceneWidth/2) / (cellSize))) * columns );

	for(int i = 0; i < kernelParticles; i++){
		particleIds[i] = -1;
	}
	for(int i = 0; i < kernelParticles; i++){
		tempParticleIds[i] = -1;
	}

	int *pointer;
	pointer = tempParticleIds;

	int counter = 0;
	int numberOfParticlesNearby = 0;

	for(int i = 0; i < 4; i++){
		if(bucketIds[i] >= 0 && bucketIds[i] <= rows*columns){
			numberOfParticlesNearby += particleCounter[bucketIds[i]];
		}
	}

	for(int i = 0; i < 4; i++){
		if(bucketIds[i] >= 0 && bucketIds[i] <= rows*columns){
			for(int j = 0; j < particleCounter[bucketIds[i]]; j++){
					tempParticleIds[counter] = Buckets[bucketIds[i]][j];
					counter++;
			}
		}
	}

	//std::random_shuffle(tempParticleIds, tempParticleIds + counter);
	if(counter > kernelParticles){
		for(int i = 0; i < kernelParticles; i++){
			int randomIndex = rand() % counter;
			particleIds[i] = tempParticleIds[randomIndex];
		}
		//std::cout << counter << std::endl;
	}
	//std::cout << "particles nearby: " << counter << std::endl;
	return pointer; 
}