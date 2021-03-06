#include "Settings.h"

// SPH parameters
const float Settings::TIME_STEP = 0.005;
const float Settings::DISTANCE = 0.02;
const float Settings::PARTICLE_MASS = 0.013;
const float Settings::IDEAL_DENSITY = 1000;
const float Settings::VISCOSITY_CONST = 3.5;
//const float SURFACE_TENSION = 0.003;
//const float TENSION_THRESHOLD = 12;
const float Settings::STIFFNESS = 3;
const float Settings::H = 0.032;

// External forces
const float Settings::INPUTFORCE = 0.6;
const bool Settings::ZEROGRAVITY = false;
const float Settings::GRAVITY = -9.82;

// Bounding box
const float Settings::BOX_SIZE = 0.4;
const float Settings::ENERGYLOSS = 1.0;