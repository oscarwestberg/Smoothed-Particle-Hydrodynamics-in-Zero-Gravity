//
//  fragmentshader.frag
//  Smoothed Particle Hydrodynamics in Zero Gravity
//
//  Created by Oscar Westberg.
//  Copyright (c) 2014 Group 7 @ Linköpings University, course TNM085. All rights reserved.
//

#version 150 core

uniform vec3 triangleColor;

out vec4 outColor;

void main()
{
    outColor = vec4(triangleColor, 1.0);
}