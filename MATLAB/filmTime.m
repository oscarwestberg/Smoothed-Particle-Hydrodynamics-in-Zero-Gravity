function filmTime(particlePos)

maxPosition = 0.3;%max(particlePos(:));
filmLength = length(particlePos(1,1,:));

padding = 0.2;

for frame = 1:filmLength
    plot(particlePos(:,1,frame),particlePos(:,2,frame), 'o')
    axis([-(maxPosition + padding) (maxPosition + padding) -(maxPosition + padding) (maxPosition + padding)]);
    pause(0.01)
end