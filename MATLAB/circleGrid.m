function [positions] = circleGrid(SIZE, POS_DISTANCE)

radius = POS_DISTANCE*SIZE/2;

[x y] = meshgrid(0:POS_DISTANCE:(SIZE-1)*POS_DISTANCE, 0:POS_DISTANCE:(SIZE-1)*POS_DISTANCE);
temp_positions = [x(:)-radius, y(:)-radius];

positions = zeros(0,2);

for i = 1:length(temp_positions)
    dist = sqrt(temp_positions(i,1)^2 + temp_positions(i,2)^2);
    if(dist < radius)
        coord = [temp_positions(i,1) temp_positions(i,2)];
        positions = [positions; coord];
    end
end



%plot(positions(:,1), positions(:,2), 'o');