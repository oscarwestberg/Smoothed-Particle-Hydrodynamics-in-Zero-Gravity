function [p] = pressure(density, targetDensity)
    % k är konstant
    k = 3.5;    
    p = k*(density - targetDensity);
end