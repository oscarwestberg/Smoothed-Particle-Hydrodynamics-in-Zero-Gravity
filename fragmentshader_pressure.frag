//
//  fragmentshader.frag
//  Smoothed Particle Hydrodynamics in Zero Gravity
//

#version 150 core

#define width 800
#define height 600
#define MAX_PARTICLES 20*20

const float BOX_SIZE = 0.4;
uniform vec2[MAX_PARTICLES] positions;
uniform float[MAX_PARTICLES] pressures;

out vec4 outColor;
void main()
{
    // We're using uv coordinates in the fragment shader, the *0.5 zooms in
    vec2 uv = (gl_FragCoord.xy / vec2(width, height) -0.5)*2*(width/height)*0.5;
    float sum = 0;
    vec4 color = vec4(0.611,0.13,0.13, 1.0);
    vec4 ambientColor = vec4(0.3,0.3,0.3,1.0);
    float pressureSum = 0;
    float boundingSize = 0.02;

    // Decrease to increase the size of the metaballs, increase to decrease the size
    float threshold = 12000;
    
    // Calculates the electrical field contribution from all particles
    for(int i = 0; i < MAX_PARTICLES; i++){
        float dist = (uv.x-positions[i].x)*(uv.x-positions[i].x) + (uv.y-positions[i].y)*(uv.y-positions[i].y);
        sum += 1/dist;
        // Check if particle is near current uv coordinate
        if(((positions[i].x <= uv.x+boundingSize) && (positions[i].x >= uv.x-boundingSize)) &&
           ((positions[i].y <= uv.y+boundingSize) && (positions[i].y >= uv.y-boundingSize)))
            pressureSum += pressures[i];
    }
    
    // Normal color
    if(sum >= threshold){
        outColor = color*(pressureSum/10000);
    }
    // Background color
    else{
        outColor = ambientColor;
    }
    
    // Make sure nothing blue is drawn outside the bounding box
    if((uv.x > BOX_SIZE) || (uv.x < -BOX_SIZE) || (uv.y > BOX_SIZE) || (uv.y < -BOX_SIZE))
        outColor = ambientColor;
    
    // Draw bounding box
    if(((uv.x >= BOX_SIZE-0.001) && (uv.x <= BOX_SIZE+0.001)) && ((uv.y <= BOX_SIZE) && (uv.y >= -BOX_SIZE)) ||
       ((uv.x >= -BOX_SIZE-0.001) && (uv.x <= -BOX_SIZE+0.001)) && ((uv.y <= BOX_SIZE) && (uv.y >= -BOX_SIZE)) ||
       ((uv.y >= BOX_SIZE-0.001) && (uv.y <= BOX_SIZE+0.001)) && ((uv.x <= BOX_SIZE) && (uv.x >= -BOX_SIZE)) ||
       ((uv.y >= -BOX_SIZE-0.001) && (uv.y <= -BOX_SIZE+0.001)) && ((uv.x <= BOX_SIZE) && (uv.x >= -BOX_SIZE)))
        outColor = vec4(1,1,1,1);
}
