function [isOutside, normal, contactPoint] = outsideBox(particlePos,axis1, axis2)

%axis = [position ynormal xnormal]

posMin = axis1(1);
posMax = axis2(1);
minNormal = [axis1(2) axis1(3)];
maxNormal = [axis2(2) axis2(3)];

% X-axis
if(axis1(3) == 0)
    pos = particlePos(1);
else
    pos = particlePos(2);
end

if(pos > posMin && pos < posMax)
    isOutside = 1;
    normal = 0;
    contactPoint = 0;
elseif(pos < posMin)
    isOutside = -1;
    normal = minNormal;
    contactPoint = posMin;
elseif(pos > posMax)
    isOutside = -1;
    normal = maxNormal;
    contactPoint = posMax;
elseif(pos == posMin)
    isOutside = 0;
    normal = minNormal;
    contactPoint = posMin;
elseif(pos == posMax)
    isOutside = 0;
    normal = maxNormal;
    contactPoint = posMax;
end

