%Clear
close all;
clear all;
clc;

%Set decimal precision
format('long','g')
%Load Data
% load('TUdat.mat','TUdat');
Data = xlsread('GarminDat.xlsx');
x = Data(:,1);
y = Data(:,2);
z = Data(:,3)*-1;
%Find a size of variable "Depth"
s = size(x(:,1),1);

% %% Trisurf
% hold on
% plot3(x,y,zeros(s,1), '*r')
% axis([min(x), max(x),...
%       min(y), max(y)]);
% grid
% stem3(x,y,z,'^','fill')
% hold off
% % view(322.5, 30);
% 
% figure('Color', 'white');
% t = delaunay(x,y);
% 
% hold on
% 
% trimesh(t,x,y, zeros(s,1), ...
%     'EdgeColor','r', 'FaceColor','none')
% defaultFaceColor  = [0.6875 0.8750 0.8984];
% trisurf(t,x,y,z, 'FaceColor', ...
%     defaultFaceColor, 'FaceAlpha',0.9);
% plot3(x,y,zeros(241,1), '*r')
% axis([min(x), max(x),...
%       min(y), max(y),...
%       min(z)-0.5, max(z)+0.5]);
% grid


%% Griddata
plot3(x,y,z,'.','markersize',12)
xlabel('Latitude')
ylabel('Longitude')
zlabel('Depth in Meters')
grid on

figure
[xi,yi] = meshgrid(min(x):0.00001:max(x), min(y):0.00001:max(y));
zi = griddata(x,y,z,xi,yi);
surf(xi,yi,zi);
axis([min(x), max(x),...
      min(y), max(y),...
      min(z)-0.5, max(z)+0.5]);
xlabel('Latitude')
ylabel('Longitude')
zlabel('Depth in Meters')

figure
[c,h] = contour(xi,yi,zi);
clabel(c,h);
xlabel('Latitude')
ylabel('Longitude')