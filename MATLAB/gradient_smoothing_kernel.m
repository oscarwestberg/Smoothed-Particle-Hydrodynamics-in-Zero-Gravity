function [kw] = gradient_smoothing_kernel(r,h)
    % Ta in avståndsvektorer (r - rb)
    % Returnera kernel weight
    r_abs = sqrt(r(:,1)^2 + r(:,2)^2);
    
    kw = zeros(length(r_abs),2);
    for i = 1:length(r_abs)
        if(r_abs(i)~=0 && r_abs(i) < h)
            kw(i,:) = (-45/(3.14*h^6))*(h - r_abs(i))^2*(r(i,:)/r_abs(i));
        else
            kw(i,:) = [0 0];
        end
    end
end