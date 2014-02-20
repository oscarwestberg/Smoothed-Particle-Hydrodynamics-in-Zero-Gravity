struct Particle
{
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 pressureGradient;
	glm::vec3 viscosity;
	float density;
	float pressure;
};