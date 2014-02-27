//
//  fragmentshader.frag
//  Smoothed Particle Hydrodynamics in Zero Gravity
//
//  Created by Oscar Westberg.
//  Copyright (c) 2014 Group 7 @ Linköpings University, course TNM085. All rights reserved.
//

#version 150 core

#define width 800
#define height 600
#define MAX_PARTICLES 12*12


uniform vec3[MAX_PARTICLES] positions;

out vec4 outColor;

/*
For each pixel, cast a ray out
    Determine the intersection of the ray with all point charges' bounding spheres
    Sort the intersections (both entries & exits!)
    For each intersection (from nearest to farthest)
        If 'entry' (into bounding sphere): add that charge to 'active' list;
        Else ('exit' from bounding sphere): remove that charge from 'active' list.
        Now trace along the ray from this intersection to the next intersection,
        using the 'active' list as the point charges contributing to the field.

  -----------------------------
 
For each fragment, check bounding sphere tex.
if pixel is inside one of the spheres(in screen space),
use that value as z starting point for ray.

calc density for that pixel.
f density > threshold, we hit a blob and can calc normals and lighting for it.

if not, decrease z value for a reasonable stepSize until we find a value bigger >threshold, or reach a certain max depth.

 -----------------------------
 
 Ray Casting
 1. Shoot a ray through each pixel on the
 screen.
 2. Step through that ray until you reach a pixel
 that is "inside" the isosurface.
 3. Calculate normal and color the pixel
*/


void main()
{
    vec2 uv = (gl_FragCoord.xy / vec2(width, height) -0.5)*2*(width/height)*0.4;
    float sum = 0;
    bool found = false;
    float z = 1;
    float steps = 50;
    
    
    for(int i = 0; i < MAX_PARTICLES; i++){
        sum += 1/((uv.x-positions[i].x)*(uv.x-positions[i].x) + (uv.y-positions[i].y)*(uv.y-positions[i].y));
    }
    
    float threshold = 50000;
    
    if(sum >= threshold){
        sum = 0;
        // Check depth
        for(int ray = 0; ray < steps; ray++){ // Cast ray
            z = 1 - ray/(steps*0.5);
            if(!found){
                for(int i = 0; i < MAX_PARTICLES; i++){
                    if(!found){
                        sum += 1/((uv.x-positions[i].x)*(uv.x-positions[i].x) + (uv.y-positions[i].y)*(uv.y-positions[i].y) + (z-positions[i].z)*(z-positions[i].z));
                        if(sum >= threshold){
                            found = true;
                            float temp = 0;
                            z = z*0.5 + 0.5;
                            
                            outColor = vec4(z*3.0,z*1.8,z*1.8,1.0);
                        }
                    }
                }
            }
        }
        
        
    }
    
    else{
        outColor = vec4(0.0,0.0,0.0,1.0);
    }
}


/*
void main(void)
{
    vec2 uv = (gl_FragCoord.xy / vec2(width, height) -0.5)*2*(width/height);
    float sum = 0;
    float threshold = 4;
    int stepSize = 20;
    float z = -1;
    bool found = false;
    
    for(int ray = 0; ray < stepSize; ++ray){ // Cast ray
        if(!found){
            z = ray/(stepSize/2) -1;
            
            for(int i = 0; i < MAX_PARTICLES; i++){
                if(!found){
                    sum += 1/((uv.x-positions[i].x)*(uv.x-positions[i].x) + (uv.y-positions[i].y)*(uv.y-positions[i].y) + (z-positions[i].z)*(z-positions[i].z));
                    
                    if(sum > threshold){ // Found blob
                        // Calculate normals etc
                        sum *= 0.0009;
                        outColor = vec4(sum,sum,sum,1.0);
                        found = true;
                        break;
                    }
                }
            }
            sum = 0;
        }
    }
    
    if(!found)
        outColor = vec4(0,0,0,1);
    
}
*/


/*
void main()
{
    vec2 uv = (gl_FragCoord.xy / vec2(width, height) -0.5)*2*(width/height);
    float sum = 0;
    float threshold = 0.2;
    
    for(int ray = 0; ray < 10; ++ray){
        float z = ray/5 -1;
        int outsideInside[3];
        outsideInside[0] = 0;
        outsideInside[1] = 0;
        outsideInside[2] = 0;
        
        for(int i = 0; i < MAX_PARTICLES; i++){
            
            // vec3(uv, z) - At the end of the ray
            float r = distance(vec3(uv,z), positions[i]);
            //float temp = r * r * r * (r * (r * 6 - 15) + 10); // Bounding sphere
            
            if((r >= threshold) && (outsideInside[1] == 0)){ // Before blob
                outsideInside[0] = i;
            }
            if(r < threshold){ // Inside blob
                outsideInside[1] = i;
            }
            if((outsideInside[0] != 0) && (outsideInside[1] != 0)){ // Found an entry
                sum += r;
                outsideInside[0] = 0;
                outsideInside[1] = 0;
            }
        }
    }
    
    outColor = vec4(sum,sum,sum,1.0);
}

*/
/*
void main()
{
    vec2 uv = (gl_FragCoord.xy / vec2(width, height) -0.5)*2*(width/height);
    float sum = 0;
    
    for(int i = 0; i < MAX_PARTICLES; i++){
        // Replace with more efficient function?
        sum += 1/((uv.x-positions[i].x)*(uv.x-positions[i].x) + (uv.y-positions[i].y)*(uv.y-positions[i].y));
    }
    
    float threshold = 4000;

    if(sum >= threshold)
        outColor = vec4(1.0,1.0,1.0,1.0);
    else{
        sum /= threshold;
        //outColor = vec4(0.0,0.0,0.0,1.0);
        outColor = vec4(sum,sum,sum,1.0);
    }
    
}
*/