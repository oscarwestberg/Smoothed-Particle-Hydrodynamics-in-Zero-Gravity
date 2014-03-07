//
//  fragmentshader.frag
//  Smoothed Particle Hydrodynamics in Zero Gravity
//

#version 150 core

#define width 800
#define height 600
#define MAX_PARTICLES 20*20
#define MAX_BRICK 15*30

uniform float colorOfBrick[MAX_BRICK];

out vec4 outColor;
void main()
{
    // We're using uv coordinates in the fragment shader, the *0.5 zooms in
    vec2 uv = (gl_FragCoord.xy / vec2(width, height) -0.5)*2*(width/height)*0.5;
    float sum = 0;
    vec4 color = vec4(0.0,0.0,0.9, 1.0);

    vec4 ambientColor = vec4(0.2,0.2,0.2,1.0);
    float threshold = 600;
	float BOX_SIZE = 0.4;

	 // Make sure nothing blue is drawn outside the bounding box
    if((uv.x > BOX_SIZE) || (uv.x < -BOX_SIZE) || (uv.y > BOX_SIZE) || (uv.y < -BOX_SIZE)){

        outColor = ambientColor;
    }else{

    // Draw bounding box
    if(((uv.x >= BOX_SIZE-0.001) && (uv.x <= BOX_SIZE+0.001)) && ((uv.y <= BOX_SIZE) && (uv.y >= -BOX_SIZE)) ||
       ((uv.x >= -BOX_SIZE-0.001) && (uv.x <= -BOX_SIZE+0.001)) && ((uv.y <= BOX_SIZE) && (uv.y >= -BOX_SIZE)) ||
       ((uv.y >= BOX_SIZE-0.001) && (uv.y <= BOX_SIZE+0.001)) && ((uv.x <= BOX_SIZE) && (uv.x >= -BOX_SIZE)) ||
       ((uv.y >= -BOX_SIZE-0.001) && (uv.y <= -BOX_SIZE+0.001)) && ((uv.x <= BOX_SIZE) && (uv.x >= -BOX_SIZE))) {
		outColor = vec4(1,1,1,1);
		} 
		else {

		int bricksWide = 10;
		float sceneWidth = 0.8;
		float sceneHeight = 0.8;
		float legoHeight = 0.8/(bricksWide*2);
		float legoWidth = 0.8/bricksWide;

		int brickId1 = int(floor(uv.x / legoWidth + bricksWide/2));
		int brickId2 = int(floor(uv.y / legoHeight + (bricksWide*2)/2));
		int brickId3 = brickId1 + brickId2*bricksWide; 

		if(brickId3 > 0 && brickId3 < MAX_BRICK){
	
			sum = colorOfBrick[brickId3];
			//outColor = color;
			if(sum >= threshold){ // Normal color
				outColor = ambientColor + color*0.9;
			}
			else if((sum < threshold) && (sum > threshold*0.8)){ // Borders
				outColor = ambientColor + color*0.7;
			}
			else{
				outColor = ambientColor;
			}
		} else{
			outColor = ambientColor;
		}   
	}
}
}
