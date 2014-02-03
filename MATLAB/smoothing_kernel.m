function [kw] = smoothing_kernel(r,h)
    % Ta in avståndsvektorer (r - rb)
    % Returnera kernel weight
    
    r_abs = sqrt(r(:,1).^2 + r(:,2).^2);
    
    kw = zeros(length(r_abs),1);
    for i = 1:length(r_abs)
        if(r_abs(i) < h)
            kw(i) = (315/(64*3.14*h^9))*((h^2 - r_abs(i)^2)^3);
        else
            kw(i) = 0;
        end
    end
   % sum(kw)
end