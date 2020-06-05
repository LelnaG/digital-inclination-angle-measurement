delete(instrfindall); %closes any ports that may be open

xwidth = inf;
delay = 0.1;
time = 0;
angle = 0;
counter = 0;

plotGraph = plot(time,angle);
title('Damilola Orole 400065299');
xlabel('Time(s)');
ylabel('Angle(degrees)');
xlim([0 60]);
ylim([0 90]);
grid on;

s = serial('COM3','BaudRate',38400); % set serial settings
fopen(s);%open serial communication
tic %start timer


while (time<xwidth)
    input = (fscanf(s,'%d')); %read in calculated angle values
    
    if(~isempty(input) && isfloat(input)) %check that input is not empty
        counter = counter + 1;
        time(counter) = toc; %set time = timer offset(approx. 10ms per sample)
        angle(counter) = input(1);
        
    if(xwidth > 0)
        set(plotGraph,'XData',time(time > time(counter)-xwidth), 'YData', angle(time > time(counter)-xwidth));
%update scrolling axis

    else
    set(plotGraph,'XData',time,'YData',angle);
    
    end
    pause(delay);
   end
end

fclose(s);