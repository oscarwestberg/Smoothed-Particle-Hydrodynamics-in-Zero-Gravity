function [positions] = circleFormation(circles, POS_DISTANCE)
%clear all
%circles = 10;
%POS_DISTANCE = 0.01;


nrpoints = zeros(1,circles);
nrpoints(1) = 1;

for i = 2:circles
    nrpoints(i) = nrpoints(i-1) + 2;
end

nrpoints = nrpoints*4;

positions = zeros(sum(nrpoints(:)),2);
positions(1,:) = [0 0];

count = 2;
for i = 2:length(nrpoints)
    for j = 1:nrpoints(i)
        steps = nrpoints(i);
        positions(count,1) = (i*POS_DISTANCE-POS_DISTANCE)*cos((j/steps)*2*pi);
        positions(count,2) = (i*POS_DISTANCE-POS_DISTANCE)*sin((j/steps)*2*pi);
        count = count + 1;
    end
end

%plot(positions(:,1), positions(:,2), 'o');