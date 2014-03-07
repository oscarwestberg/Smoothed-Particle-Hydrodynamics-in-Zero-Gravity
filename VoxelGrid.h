#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "Particle.h"
#include "Settings.h"

class VoxelGrid
{   
	public:		
		int counter;
		float cellSize;
		float sceneWidth;
		float sceneHeight;
		float H;
		int particleCounter[Settings::ROWS*Settings::COLUMNS];

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

		int Buckets[Settings::ROWS*Settings::COLUMNS][Settings::MAXPARTICLESINCELL];
		int particleIds[Settings::KERNELPARTICLES];
		int tempParticleIds[Settings::MAXPARTICLESINCELL];

	//	std::vector<int> Buckets[9];
		
	private:

};