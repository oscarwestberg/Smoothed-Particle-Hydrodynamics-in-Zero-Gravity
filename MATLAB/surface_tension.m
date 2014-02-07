function [surfaceTension] = surface_tension(particleMass, densityVec, particlePos, sigma, l, h)


inward_surface_normal = zeros(length(particlePos),2);
laplacian_tension_quantity = zeros(length(particlePos),2);

normalized_normal = zeros(size(inward_surface_normal));

for i = 1:length(particlePos)
    coord = particlePos(i,:);
    for j = 1:length(particlePos)    
        dist_vec = [ (coord(1) - particlePos(j,1)) (coord(2)- particlePos(j,2))];
        massDensity = (particleMass/densityVec(j));
        %laplacian_smoothing_kernel(dist_vec,h)
        inward_surface_normal(i,:) = inward_surface_normal(i,:) + massDensity*gradient_smoothing_kernel(dist_vec, h); 
        laplacian_tension_quantity(i,:) = laplacian_tension_quantity(i,:) +  massDensity*laplacian_smoothing_kernel(dist_vec, h);
        
        normal_abs = sqrt(inward_surface_normal(i,1)^2 + inward_surface_normal(i,2)^2);
        
        if(normal_abs >= l)
            normalized_normal(i,:) = inward_surface_normal(i,:)./normal_abs;
        else
            normalized_normal(i,:) = [0 0];
        end
    end        
end
%inward_surface_normal = inward_surface_normal;
%normalized_normal = normalized_normal;
surfaceTension = zeros(size(normalized_normal));

%laplacian_tension_quantity = laplacian_tension_quantity;
surfaceTension = -sigma*laplacian_tension_quantity.*normalized_normal;
