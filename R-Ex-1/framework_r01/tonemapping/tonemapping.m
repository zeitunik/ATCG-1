%% prepare workspace
clear
close all
clc
load Buddha.mat

%% linear scaling
I_linear = I;

% implement your method for linear scaling here:

fh = figure();
ax1 = subplot(2, 2, 1);
imshow(max(0, min(1, I)));
title(ax1, 'original');

ax2 = subplot(2, 2, 2);
imshow(max(0, min(1, I_linear)));
title(ax2, 'linear scaling');

%% gamma correction
I_gamma  = I;
gamma    = 1/4; % try correction with this gamma value

% implement your method for gamma correction here:

ax2 = subplot(2, 2, 3);
imshow(max(0, min(1, I_gamma)));
title(ax2, 'gamma correction');

%% histogram adjustment
I_hist = I;
nbins = 256; %number of bins

%use this box filtered image as an approximation of the 1 degree fov image
boxfilter = fspecial('average', [9 9]);
I_filtered = imfilter(I, boxfilter);

% implement your method for histogram scaling here:
% ...
% hint: have a look at the functions "hist", "cumsum" and "interp1".
% hint: You might want to perform the actual tonemapping by transforming the RGB
% image into the HSV color space and then only work on the V channel. Transform
% the tonemapped image back to RGB afterwards. For this you can use Matlab's
% rgb2hsv() and hsv2rgb() functions.

ax4 = subplot(2, 2, 4);
imshow(max(0,min(1,I_hist)));
title(ax4, 'histogram adjustment');
