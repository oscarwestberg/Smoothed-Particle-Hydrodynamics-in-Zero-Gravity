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
		float cellSize;
		float sceneWidth;
		float sceneHeight;
		static const int rows = 7;
		static const int columns = 7;
		int particleCounter[rows*columns];
		float H;
		static const int maxParticlesInCell = 100;
		static const int kernelParticles = 100;

		void getNeighbors();
		void Setup(float scenewidth, float sceneheight, float cellsize);
		std::list<int> AddBucket(glm::vec2 vector,float width, std::list<int> buckettoaddto);
		void RegisterObject(const Particle& particle, int particleId);
		void ClearBuckets();

		std::list<int> GetIdForObj(const Particle& particle);

		int* GetNearby(const Particle& particle);
		
		// std::map<int, std::vector<Particle>> Voxel;
		std::map<int, int> indexCounter;
		//std::map<int  , std::vector<int>> Buckets;

		int Buckets[rows*columns][maxParticlesInCell];
		int particleIds[kernelParticles];

	//	std::vector<int> Buckets[9];
		
	private:

};