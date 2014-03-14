//
//  vertexshader.frag
//  Smoothed Particle Hydrodynamics in Zero Gravity
//
//  Created by Oscar Westberg.
//  Copyright (c) 2014 Group 7 @ Linköpings University, course TNM085. All rights reserved.
//

#version 150 core

in vec3 position;

void main() {
    gl_Position = vec4(position, 1.0);
}