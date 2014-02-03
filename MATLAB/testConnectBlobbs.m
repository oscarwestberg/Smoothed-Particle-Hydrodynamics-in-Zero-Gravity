%% Skapar massa randomkoordinater
SIZE = 400;
rand_x = randi([1 SIZE], 1, SIZE*2);
rand_y = randi([1 SIZE], 1, SIZE*2);

% Vilket håll färdas partiklarna?
randDir_x = randi([-1 1], 1, SIZE*2);
randDir_y = randi([-1 1], 1, SIZE*2);

randPos = [rand_x' rand_y' randDir_x' randDir_y'];

%Plotta bara positionerna inte randDir_x eller randDir_y
% plot(randPos(:, 1:2), 'x');

%% Skapa matris med alla particlar
M = zeros(SIZE, SIZE);

for i = 1:length(randPos)
    pos = randPos(i,:);
    M(pos(1), pos(2)) = 1;
end

%% Länka koordinater som är nära varandra
coverRadius = 10;
for times = 1:200
    for i = 1:length(randPos)
	pos = randPos(i,:);
        
        if(((pos(1) - coverRadius) > 0 && (pos(2) - coverRadius) > 0 && pos(1) + coverRadius < SIZE && pos(2) + coverRadius < SIZE))
            if(M(pos(1) + 1,pos(2)) == 0 && M(pos(1) - 1, pos(2)) == 0 && M(pos(1), pos(2) + 1) == 0 && M(pos(1), pos(2) - 1) == 0)
                radius = 1;
                while radius < coverRadius 
                    if (M(pos(1) + radius, pos(2)) == 1)
                        randPos(i,3) = 1;
                        randPos(i,4) = 0;
                        break;
                    end

                    if (M(pos(1) - radius, pos(2)) == 1)
                        randPos(i,3) = -1;
                        randPos(i,4) = 0;
                        break;
                    end

                    if (M(pos(1), pos(2) + radius) == 1)
                        randPos(i,3) = 0;
                        randPos(i,4) = 1;
                        break;
                    end

                    if (M(pos(1), pos(2) - radius) == 1)
                        randPos(i,3) = 0;
                        randPos(i,4) = -1;
                        break;
                    end

                    if (M(pos(1) + radius, pos(2) + radius) == 1)
                        randPos(i,3) = 1;
                        randPos(i,4) = 0;
                        break;
                    end

                    if (M(pos(1) - radius, pos(2) + radius) == 1)
                        randPos(i,3) = -1;
                        randPos(i,4) = 0;
                        break;
                    end

                    if (M(pos(1) - radius, pos(2) - radius) == 1)
                        randPos(i,3) = 0;
                        randPos(i,4) = 1;
                        break;
                    end

                    if (M(pos(1) + radius , pos(2) - radius) == 1)
                        randPos(i,3) = 0;
                        randPos(i,4) = -1;
                        break;
                    end
                    radius = radius + 1;
                end
            end
        end
    end

    % Adderar direction vektorn till pixeln och ritar ut 
    for i = 1:length(randPos)
        pos = randPos(i,:);
        % Vart ska den vara nästa ruta? 
        newPos_x = pos(1) + pos(3);
        newPos_y = pos(2) + pos(4);
        
        if(newPos_x > 0 && newPos_y > 0 && newPos_x < SIZE && newPos_y < SIZE)
            M(newPos_x, newPos_y) = 1;
            M(pos(1), pos(2)) = 0;

            randPos(i,1) = newPos_x;
            randPos(i,2) = newPos_y;
        end
    end
    imshow(M);
    pause(0.0001);
end
