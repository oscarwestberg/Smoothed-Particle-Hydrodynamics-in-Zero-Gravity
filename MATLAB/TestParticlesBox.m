%% Skapar massa randomkoordinater
clear all;
SIZE = 10;
POS_DISTANCE = 0.03;
TIME_STEP = 0.01;
particleMass = 0.08;
idealDensity = 1000;
gravity = [0 -9.82];
viscosityConst = 3.5;
surface_tension_const = 0.0728;
threshold_l = 12.235;
h = 0.065;
axisMax = 0.5;

% Bounding box axis [position normal]
boxLeft = -0.3;
boxRight = 0.3;
boxTop = 0.3;
boxBottom = -0.3;

axisX1 = [boxLeft [1 0]];
axisX2 = [boxRight [-1 0]];
axisY1 = [boxBottom [0 1]];
axisY2 = [boxTop [0 -1]];

[x y] = meshgrid(0:POS_DISTANCE:(SIZE-1)*POS_DISTANCE, 0:POS_DISTANCE:(SIZE-1)*POS_DISTANCE);
particlePos = [x(:)-POS_DISTANCE*(floor(SIZE/2)), y(:)-POS_DISTANCE*(floor(SIZE/2))];

vary_const = 0.1;
for i = 1:length(particlePos(:,1))
    particlePos(i,:) = [particlePos(i,1) + vary_const*POS_DISTANCE*(rand(1)-0.5) particlePos(i,2) + vary_const*POS_DISTANCE*(rand(1)-0.5)];
end

% fungerar inte med cirklar
%particleVelocity = zeros(SIZE^2, 2);
%particleAcceleration = zeros(SIZE^2, 2);

particleVelocity = zeros(length(particlePos), 2);
particleAcceleration = zeros(length(particlePos), 2);

iterations = 10;
particlePosSave = zeros(length(particlePos),2,iterations);
%surfaceTensionBoolSave = zeros(length(particlePos),1,iterations);
% Main loop
for i = 1:iterations
    densVec = density(particlePos,particleMass,h);
    presVec = pressure(densVec, idealDensity);
    presGradVec = pressureGradient(presVec, densVec, particlePos,particleMass, h);
    viscosity = calc_viscosity(particlePos, densVec, particleVelocity, viscosityConst, particleMass, h);
    %surfaceTension = surface_tension(particleMass, densVec, particlePos, surface_tension_const, threshold_l, h);
    % ACCELERATION
    particleAcceleration = (-presGradVec + viscosity)+ones(length(particlePos(:,1)),1)*gravity;
    %ones(length(particlePos(:,1)),1)*gravity
    % VELOCITY
    particleVelocity = particleVelocity + TIME_STEP*particleAcceleration;
    % POSITION
    particlePos = particlePos + TIME_STEP*particleVelocity;
    
    for j = 1:length(particlePos)
       [isOutsideX, xNormal, xCP] = outsideBox(particlePos(j,:),axisY1, axisY2);
       [isOutsideY, yNormal, yCP] = outsideBox(particlePos(j,:),axisX1, axisX2);
       
       if(isOutsideX ~= 1)
           contactPointX = xCP;
           penDepth = abs(contactPointX - particlePos(j,2));
           particleVelocity(j,:) = particleVelocity(j,:) - (1+1)*(dot(particleVelocity(j,:),xNormal))*xNormal;
           particlePos(j,:) = particlePos(j,:) + penDepth*xNormal;
       end
       if(isOutsideY ~= 1)
           contactPointY = yCP;
           penDepth = abs(contactPointY - particlePos(j,1));
           particleVelocity(j,:) = particleVelocity(j,:) - (1+1)*(dot(particleVelocity(j,:),yNormal))*yNormal;
           particlePos(j,:) = particlePos(j,:) + penDepth*yNormal;
       end
    end

    %surfaceTensionBoolSave(:,i) = surfaceTension(:,1) ~= 0;
    particlePosSave(:,:,i) = particlePos;
    i
    
%     plot([boxLeft boxLeft], [boxBottom boxTop])
%     hold on;
%     plot([boxRight boxRight], [boxBottom boxTop])
%     plot([boxLeft boxRight], [boxBottom boxBottom])
%     plot([boxLeft boxRight], [boxTop boxTop])
%     plot(particlePos(:,1),particlePos(:,2), 'o')
%     axis([-0.1 axisMax -0.1 axisMax])
%     hold off;
%     pause(0.01)
end