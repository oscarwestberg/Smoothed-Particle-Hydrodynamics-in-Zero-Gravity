#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "Particle.h"

class VoxelGrid
{   
	public:		
		int counter;
		float cellSize;
		float sceneWidth;
		float sceneHeight;
		float H;
		static const int maxParticlesInCell = 400;
		static const int kernelParticles = 32;
		static const int rows = 12;
		static const int columns = 12;
		int particleCounter[rows*columns];

		void getNeighbors();
		void Setup(float scenewidth, float sceneheight, float cellsize);
		std::list<int> AddBucket(glm::vec2 vector,float width, std::list<int> buckettoaddto);
		void RegisterObject(const Particle& particle, int particleId);
		void ClearBuckets();

		std::list<int> GetIdForObj(const Particle& particle);

		int* GetNearby(const Particle& particle);
		int getNumberOfNeighbouringParticles();
		
		// std::map<int, std::vector<Particle>> Voxel;
		std::map<int, int> indexCounter;
		//std::map<int  , std::vector<int>> Buckets;

		int Buckets[rows*columns][maxParticlesInCell];
		int particleIds[kernelParticles];
		int tempParticleIds[maxParticlesInCell];

	//	std::vector<int> Buckets[9];
		
	private:

};