function [visc] = calc_viscosity(particlePos, density, velocity, visc_constant,particleMass, h)

visc = zeros(length(density(:,1)),2);

for i = 1:length(visc(:,1))
    temp_coord = particlePos(i,:);
    temp_dist_vec = [(particlePos(:,1) - temp_coord(1)) (particlePos(:,2) - temp_coord(2))];
    temp_laplace = laplacian_smoothing_kernel(temp_dist_vec, h);
    temp_visc = [0 0];
    for j = 1:length(visc(:,1))
        temp_visc = temp_visc + ((velocity(j,:) - velocity(i,:))/density(j))*temp_laplace(j);
    end
    visc(i,:) = (visc_constant*particleMass/density(i))*temp_visc;
end