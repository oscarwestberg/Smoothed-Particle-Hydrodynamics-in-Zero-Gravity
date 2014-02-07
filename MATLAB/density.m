function [f] = density(randCoord,particleMass,h)
    % ta in en punkt      

    f = zeros(length(randCoord(:,1)),1);
    for i = 1:length(randCoord(:,1))
        coord = randCoord(i,:);    
        %dist = sqrt((randCoord(:,1) - coord(1)).^2 + (randCoord(:,2) - coord(2)).^2);
        dist_vec = [coord(1)-(randCoord(:,1)) ( coord(2)-randCoord(:,2))];
        f(i) = particleMass*sum(smoothing_kernel(dist_vec,h));
    end
end