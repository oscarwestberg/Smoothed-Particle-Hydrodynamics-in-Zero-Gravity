//
//  Game.cpp
//  Smoothed Particle Hydrodynamics in Zero Gravity
//
//  Created by Oscar Westberg on 2014-01-24.
//  Copyright (c) 2014 Group 7. All rights reserved.
//

#include "Game.h"

Game::Game()
{
    running = true;
    
    // Create the main window
    window.create(sf::VideoMode(800, 600), "SFML window");
}

void Game::run()
{
    /* Loop until the user closes the window */
    while (window.isOpen() && running)
    {
        /* Poll for and process events */
        checkEvents();
        update();
        render();
    }
}

void Game::checkEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Close window : exit
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        // Escape pressed : exit
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window.close();
        }
    }
    
}

void Game::update()
{
    
}

void Game::render()
{
    // Clear screen
    window.clear();

    // Update the window
    window.display();
}