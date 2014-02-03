function [presGradVec] = pressureGradient(presVec, densityVec, randCoord, particleMass, h)
    
presGradVec = zeros(length(presVec),2);
    for i = 1:length(presVec)
        coord = randCoord(i,:);
        for j = 1:length(presVec)    
            %dist = sqrt((randCoord(i,1) - coord(1)).^2 + (randCoord(i,2) - coord(2)).^2);
            dist_vec = [(randCoord(j,1) - coord(1)) (randCoord(j,2) - coord(2))];
            %presGradVec(j,:) = presGradVec(j,:) + densityVec(j)*(presVec(i) - presVec(j))*gradient_smoothing_kernel(dist);
            
            presGradVec(i,:) = presGradVec(i,:) +  particleMass*((presVec(i)/(densityVec(i))^2) + (presVec(j)/( densityVec(j))^2))*gradient_smoothing_kernel(dist_vec,h);
        end
    end
end

%