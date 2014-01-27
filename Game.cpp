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
    std::cout << "constructor" << std::endl;
    running = true;
    
    // Create the main window
    window.create(sf::VideoMode(800, 600), "SFML window");
}

void Game::run()
{
    std::cout << "run" << std::endl;
    initOpenGL();
    
    /* Loop until the user closes the window */
    while (window.isOpen() && running)
    {
        /* Poll for and process events */
        checkEvents();
        update();
        render();
    }
}

void Game::initOpenGL()
{
    std::cout << "initOpenGL" << std::endl;
    
    const GLchar* vertexSource = "Shaders/vertexshader.txt";
    const GLchar* fragmentSource = "Shaders/fragmentshader.txt";
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    float vertices[] = {
        0.0f,  0.5f, // Vertex 1 (X, Y)
        0.5f, -0.5f, // Vertex 2 (X, Y)
        -0.5f, -0.5f  // Vertex 3 (X, Y)
    };
    
    // Vao - these store the links between VBOs and the raw vertex data
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Used to upload data to the graphics card
    // OpenGL handles the memory, so all we need is an int pointing to it
    GLuint vbo; // Vertex buffer object
    glGenBuffers(1, &vbo); // Generate 1 buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // Upload data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertex data to buffer
    
    // --------------------------------------------
    // Compile and link shaders
    // --------------------------------------------
    
    // Load and compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    
    // Check if shader was compiled correctly
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    
    // Connect the shaders, make them a unified program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    // Fragment shaders can have several outputs, so we have to define which one we're looking for
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    
    // Actually use our shader program
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Tell OpenGL how our vertices are ordered
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    // Specify how the data is gathered from the vertex array
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
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
    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw a triangle from the 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // Swap buffers
    window.display();
}