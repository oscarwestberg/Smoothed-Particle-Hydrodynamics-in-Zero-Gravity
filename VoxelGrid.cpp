
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
	columns = (int)floor(scenewidth / (H*2));
    rows = (int)floor(sceneheight / (H*2));
	sceneWidth = scenewidth;
    sceneHeight = sceneheight;

	cellSize = sceneHeight/rows;
	
    for (int i = 0; i < columns*rows; i++) {
        Buckets[i];
    }
}

void VoxelGrid::RegisterObject(const Particle& particle, int particleId)
{
		int cellId = (int)(floor((particle.pos.x + (sceneWidth/2))/ (cellSize)) + (floor((particle.pos.y + (sceneHeight/2))/ (cellSize) ) * columns )); 
		Buckets[cellId].push_back(particleId);

}

void VoxelGrid::ClearBuckets() {
	Buckets.clear();

	for (int i = 0; i < columns*rows; i++) {
		Buckets[i];
	}
}

/*
std::list<int> VoxelGrid::GetIdForObj(const Particle& particle){
	std::list<int> bucketsObjIsIn;
    H = 0.07;
	glm::vec2 min = glm::vec2(
		particle.pos.x - H,
		particle.pos.y - H);   
	glm::vec2 max = glm::vec2(
		particle.pos.x + H,
		particle.pos.y + H);

	//	float width = sceneWidth / cellSize;
	float width = 0.8/0.07;  
	
	//TopLeft
	bucketsObjIsIn = AddBucket(min, width, bucketsObjIsIn);

	//TopRight
	bucketsObjIsIn = AddBucket(glm::vec2(max.x, min.y), width, bucketsObjIsIn);

	//BottomRight
	bucketsObjIsIn = AddBucket(glm::vec2(max.x, max.y), width, bucketsObjIsIn);
	
	//BottomLeft
	bucketsObjIsIn = AddBucket(glm::vec2(min.x, max.y), width, bucketsObjIsIn);
	
	return bucketsObjIsIn;    
}
*/
std::list<int> VoxelGrid::GetNearby(const Particle& particle) {
	std::list<int> objects;
	
	int bucketIds[4];

	bucketIds[0] = (int)( (floor((particle.pos.x + cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y + cellSize/2 + sceneWidth/2) / (cellSize))) * columns );
	bucketIds[1] = (int)( (floor((particle.pos.x + cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y - cellSize/2 + sceneWidth/2) / (cellSize))) * columns );
	bucketIds[2] = (int)( (floor((particle.pos.x - cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y - cellSize/2 + sceneWidth/2) / (cellSize))) * columns );
	bucketIds[3] = (int)( (floor((particle.pos.x - cellSize/2 + sceneWidth/2)/ (cellSize))) + (floor((particle.pos.y + cellSize/2 + sceneWidth/2) / (cellSize))) * columns );

	for(int i = 0; i < 4; i++){
		for(std::list<int>::iterator itBucket = Buckets[bucketIds[i]].begin(); itBucket != Buckets[bucketIds[i]].end(); itBucket++){
			objects.push_back((*itBucket));
		}				
	}

	return objects;   
}