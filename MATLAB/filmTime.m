function filmTime(particlePos, particleVector)

maxPosition = 1.2;%max(particlePos(:));
filmLength = length(particlePos(1,1,:));
r = 0.25;
padding = 0.2;
STEPS = 180;

for frame = 1:filmLength
    plot(particlePos(:,1,frame),particlePos(:,2,frame), 'o')
    hold on;
    quiver(particlePos(:,1,frame), particlePos(:,2,frame), particleVector(:,1,frame), particleVector(:,2,frame));
    axis([-maxPosition maxPosition -maxPosition maxPosition]);
    %axis([-0.6 0.6 -0.6 0.6]);
%     for i = 0:STEPS
%         circle(i+1,1) = r*cos((i/STEPS)*2*pi);
%         circle(i+1,2) = r*sin((i/STEPS)*2*pi) - 0.5;
%     end
%     plot(circle(:,1), circle(:,2), 'k');
    hold off;
    pause(0.01)
end