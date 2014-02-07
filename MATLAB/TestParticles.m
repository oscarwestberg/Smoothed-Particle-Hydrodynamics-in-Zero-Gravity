%% Skapar massa randomkoordinater
clear all;
SIZE = 10;
POS_DISTANCE = 0.07;
TIME_STEP = 0.01;
particleMass = 0.08;
idealDensity = 1000;
gravity = [0 -9.82];
% viscosityConst = 3.5;
viscosityConst = 17.5;
surface_tension_const = 0.0728;
threshold_l = 12.235;
% h = 0.065;
h = 0.095;
axisMax = 0.5;

%[x y] = meshgrid(0:POS_DISTANCE:(SIZE-1)*POS_DISTANCE, 0:POS_DISTANCE:(SIZE-1)*POS_DISTANCE);
%x = randi(10, [10, 10]);
%y = randi(10, [10, 10]);

circles = 3;

%particlePos = circleFormation(circles, POS_DISTANCE); %[x(:), y(:)];
particlePos = circleGrid(SIZE, POS_DISTANCE); %[x(:), y(:)];

vary_const = 0.1;
for i = 1:length(particlePos(:,1))
    particlePos(i,:) = [particlePos(i,1) + vary_const*POS_DISTANCE*(rand(1)-0.5) particlePos(i,2) + vary_const*POS_DISTANCE*(rand(1)-0.5)];
end

% fungerar inte med cirklar
%particleVelocity = zeros(SIZE^2, 2);
%particleAcceleration = zeros(SIZE^2, 2);

particleVelocity = zeros(length(particlePos), 2);
particleAcceleration = zeros(length(particlePos), 2);

iterations = 170;
particlePosSave = zeros(length(particlePos),2,iterations);
surfaceTensionBoolSave = zeros(length(particlePos),1,iterations);
particleAccelerationSave = zeros(length(particlePos),2,iterations);
particleVelocitySave = zeros(length(particlePos),2,iterations);

% Main loop
for i = 1:iterations
    densVec = density(particlePos,particleMass,h);
    % HAHA, PRESSVÄCK!
    presVec = pressure(densVec, idealDensity);

    presGradVec = pressureGradient(presVec, densVec, particlePos,particleMass, h);
    viscosity = calc_viscosity(particlePos, densVec, particleVelocity, viscosityConst, particleMass, h);
    surfaceTension = surface_tension(particleMass, densVec, particlePos, surface_tension_const, threshold_l, h);
    particleAcceleration = -presGradVec + viscosity + surfaceTension;
    particleAccelerationSave(:,:,i) = particleAcceleration;
    %ones(length(particlePos(:,1)),1)*gravity
    particleVelocity = particleVelocity + TIME_STEP*particleAcceleration;
    particleVelocitySave(:,:,i) = particleVelocity;
    particlePos = particlePos + TIME_STEP*particleVelocity;

    %scatter(x,y,T./max(T) * 100, T,'filled')
    surfaceTensionBoolSave(:,i) = surfaceTension(:,1) ~= 0;
    particlePosSave(:,:,i) = particlePos;
    i
    %plot(particlePos(:,1),particlePos(:,2), 'o')
    %axis([-axisMax axisMax -axisMax axisMax])
    %pause
end