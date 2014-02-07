//
//  fragmentshader.frag
//  Smoothed Particle Hydrodynamics in Zero Gravity
//
//  Created by Oscar Westberg.
//  Copyright (c) 2014 Group 7 @ Linköpings University, course TNM085. All rights reserved.
//

#version 150 core

#define stiffness 30.0 // Blobbiness factor (higher = stiffer balls)
#define cfalloff 30.0 // Glow factor (higher = sharper, shorter glow)
#define MAX_PARTICLES 300
#define width 800
#define height 600

#define globes 250.
#define globesize .13
#define dist 1.2
#define persp .8

uniform vec3[MAX_PARTICLES] positions;

out vec4 outColor;


void main()
{
    vec2 uv = gl_FragCoord.xy / vec2(width, height);
	
	uv -= 0.5;  //center it
	uv *= 2.0;  //screen domain is now -1 to 1
	uv.x *= width/height; //correct for aspect ratio by varying length of horizontal axis
    
    float sum = 0;
    
    for(int i = 0; i < MAX_PARTICLES; i++){
        float temp = (uv.x-positions[i].x)*(uv.x-positions[i].x) + (uv.y-positions[i].y)*(uv.y-positions[i].y);
        if(temp == 0)
            sum += 1;
        if(temp != 0)
            sum += 1/temp;
    }
    
    float threshold = 4000;
	// calculate colour
	vec3 final;
    if(sum <= threshold)
        final = vec3(0.0,0.0,0.0);
    if(sum >= threshold)
        final = vec3(sum,sum,sum)/1000;
	
	outColor = vec4(final,1.0);
}

/*
void main()
{
    vec2 uv = gl_FragCoord.xy / vec2(width, height);
	
	uv -= 0.5;  //center it
	uv *= 2.0;  //screen domain is now -1 to 1
	uv.x *= width/height; //correct for aspect ratio by varying length of horizontal axis

	// calc range-based per-pixel values
	// subtract from length to make the ball bigger (every pixel closer)
	// clamp to avoid negative distances and fucky values
	// invert it so it's "closeness" to the ball
	// raise to power to "sharpen" the edge of the ball (more sudden falloff from 1.0)
    float sum = 0;
    
    for(int i = 0; i < MAX_PARTICLES; i++){
        sum += pow(1.0-clamp(length(uv-positions[i].xy)-0.02,0.0,1.5),stiffness);
    }

	// calculate colour
	vec3 final = vec3(sum,sum,sum);
	final.x = pow(sum,cfalloff);
	final.y = pow(sum,cfalloff);
	final.z = pow(sum,cfalloff);
	
	outColor = vec4(final,1.0);
}
*/