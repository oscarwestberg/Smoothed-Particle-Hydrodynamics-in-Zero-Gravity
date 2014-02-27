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
		static const int rows = 4;
		static const int columns = 4;
		int particleCounter[rows*columns];
		float H;

		void getNeighbors();
		void Setup(float scenewidth, float sceneheight, float cellsize);
		std::list<int> AddBucket(glm::vec2 vector,float width, std::list<int> buckettoaddto);
		void RegisterObject(const Particle& particle, int particleId);
		void ClearBuckets();

		std::list<int> GetIdForObj(const Particle& particle);

		/*
		int getHash(const Particle& particle){
			int gridShift = 50;
			int height = 100;
			// Fick feeling, nu kör vi!
			int x = ceil(particle.pos.x/cellSize) + gridShift;
			int y = ceil(particle.pos.y/cellSize) * 100 + gridShift;
			int z = ceil((particle.pos.z/cellSize) * 100 * 100 + gridShift);
			// z * width * height + y * width + x
			
			/*
			int rx = x;
			int ry = y << 10;
			int rz = z << 20;
			*/
		/*		int hashed = x + y;// + z;
			//int hashed = rx + ry + rz;

			return hashed;
		};
		*/

		std::vector<int> GetNearby(const Particle& particle);
		
		// std::map<int, std::vector<Particle>> Voxel;
		std::map<int, int> indexCounter;
		//std::map<int  , std::vector<int>> Buckets;

		int Buckets[rows*columns][50];

	//	std::vector<int> Buckets[9];
		
	private:

};