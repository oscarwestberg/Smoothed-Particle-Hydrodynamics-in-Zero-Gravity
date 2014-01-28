//
//  vertexshader.frag
//  Smoothed Particle Hydrodynamics in Zero Gravity
//
//  Created by Oscar Westberg.
//  Copyright (c) 2014 Group 7 @ Linköpings University, course TNM085. All rights reserved.
//

#version 150 core

in vec2 position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
    gl_Position = P * V * M * vec4(position, 0.0, 1.0);
}