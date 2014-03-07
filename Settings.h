#pragma once

class Settings
{
public:
	static const int SIZE = 20;
	static const int MAXPARTICLESINCELL = SIZE*SIZE;
	static const int KERNELPARTICLES = 32;
	static const int ROWS = 13;
	static const int COLUMNS = 13;
	
	// SPH parameters
	static const float TIME_STEP;
	static const float DISTANCE;
	static const float PARTICLE_MASS;
	static const float IDEAL_DENSITY;
	static const float VISCOSITY_CONST;
	//const float SURFACE_TENSION = 0.003;
	//const float TENSION_THRESHOLD = 12;
	static const float STIFFNESS;
	static const float H;

	// External forces
	static const float INPUTFORCE;
	static const bool ZEROGRAVITY;
	static const float GRAVITY;

	// Bounding box
	static const float BOX_SIZE;
	static const float ENERGYLOSS;
};