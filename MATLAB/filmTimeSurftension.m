function filmTimeSurftension(particlePos, surfaceTensionBool)

maxPosition = 0.3;%max(particlePos(:));
filmLength = length(particlePos(1,1,:));


padding = 0.1;

for frame = 1:filmLength
    tempsurface = [particlePos(:,1,frame).*surfaceTensionBool(:,1,frame) particlePos(:,2,frame).*surfaceTensionBool(:,1,frame)];
    plot(particlePos(:,1,frame),particlePos(:,2,frame), 'o')
    hold on;
    plot(tempsurface(:,1),tempsurface(:,2), 'ro')
    hold off;
    axis([-(maxPosition + padding) (maxPosition + padding) -(maxPosition + padding) (maxPosition + padding)]);
    pause(0.05)
end