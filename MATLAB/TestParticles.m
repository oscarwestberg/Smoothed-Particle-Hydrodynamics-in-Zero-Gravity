%% Skapar massa randomkoordinater
clear all;
%SIZE = 16;
POS_DISTANCE = 0.03;
TIME_STEP = 0.001;
particleMass = 4;
idealDensity = 1000;
gravity = [0 -9.82];
viscosityConst = 3.5;
h = 0.065;
axisMax = 0.05;

%[x y] = meshgrid(0:POS_DISTANCE:(SIZE-1)*POS_DISTANCE, 0:POS_DISTANCE:(SIZE-1)*POS_DISTANCE);
%x = randi(10, [10, 10]);
%y = randi(10, [10, 10]);

circles = 8;

particlePos = circleFormation(circles, POS_DISTANCE); %[x(:), y(:)];

vary_const = 0.1;
for i = 1:length(particlePos(:,1))
    particlePos(i,:) = [particlePos(i,1) + vary_const*POS_DISTANCE*(rand(1)-0.5) particlePos(i,2) + vary_const*POS_DISTANCE*(rand(1)-0.5)];
end

% fungerar inte med cirklar
%particleVelocity = zeros(SIZE^2, 2);
%particleAcceleration = zeros(SIZE^2, 2);

particleVelocity = zeros(length(particlePos), 2);
particleAcceleration = zeros(length(particlePos), 2);

iterations = 500;
particlePosSave = zeros(length(particlePos),2,iterations);
% Main loop
for i = 1:iterations
    densVec = density(particlePos,particleMass,h);
    % HAHA, PRESSVÄCK!
    presVec = pressure(densVec, idealDensity);

    presGradVec = pressureGradient(presVec, densVec, particlePos,particleMass, h);
    viscosity = calc_viscosity(particlePos, densVec, particleVelocity, viscosityConst, particleMass, h);
    particleAcceleration = -presGradVec + viscosity;
    %ones(length(particlePos(:,1)),1)*gravity
    particleVelocity = particleVelocity + TIME_STEP*particleAcceleration;
    
    particlePos = particlePos + TIME_STEP*particleVelocity;

    %scatter(x,y,T./max(T) * 100, T,'filled')
    particlePosSave(:,:,i) = particlePos;
    i
    %plot(particlePos(:,1),particlePos(:,2), 'o')
    %axis([-axisMax axisMax -axisMax axisMax])
    %pause
end