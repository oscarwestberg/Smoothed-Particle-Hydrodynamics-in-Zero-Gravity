function [p] = pressure(density, targetDensity)
    % k är konstant
    k = 3;    
    p = k*(density - targetDensity);
end