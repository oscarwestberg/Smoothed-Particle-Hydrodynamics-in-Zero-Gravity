//
//  Game.h
//  Smoothed Particle Hydrodynamics in Zero Gravity
//
//  Created by Oscar Westberg on 2014-01-24.
//  Copyright (c) 2014 Group 7. All rights reserved.
//

#ifndef __Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Game__
#define __Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Game__

// Link statically with GLEW
#define GLEW_STATIC

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <iostream>
#include <string>


class Game
{
public:Game();
	void run();
    
private:
    void initOpenGL();
	void update();
	void render();
    void checkEvents();
    
    sf::RenderWindow window;
    int width, height;
    bool running;
};

#endif /* defined(__Smoothed_Particle_Hydrodynamics_in_Zero_Gravity__Game__) */
