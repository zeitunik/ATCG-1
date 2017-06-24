# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import math
import matplotlib.pyplot as plt
import numpy as np
import scipy.io as io
import scipy.signal as sig

def rgb2hsv(rgb):
    rgb = np.float32(rgb)
    r, g, b = rgb[:, :, 0], rgb[:, :, 1], rgb[:, :, 2]
    mx = np.max(rgb, axis=2)
    mn = np.min(rgb, axis=2)
    
    df = mx - mn
    
    h = np.zeros(rgb.shape[0 : 2])
    s = np.zeros(rgb.shape[0 : 2])
    v = np.zeros(rgb.shape[0 : 2])
    h[mx == r] = np.remainder(60 * ((g[mx == r] - b[mx == r]) / df[mx == r]) + 360., 360)
    h[mx == g] = np.remainder(60 * ((b[mx == g] - r[mx == g]) / df[mx == g]) + 120., 360)
    h[mx == b] = np.remainder(60 * ((r[mx == b] - g[mx == b]) / df[mx == b]) + 240., 360)
    h[mx == mn] = 0
    s[mx != 0] = df[mx != 0] / mx[mx != 0]
    v = mx
    return np.concatenate((np.reshape(h, h.shape + (1,)), 
                           np.reshape(s, s.shape + (1,)),
                           np.reshape(v, v.shape + (1,))), axis=2)

def hsv2rgb(hsv):
    hsv = np.float32(hsv)
    h = hsv[:, :, 0]
    s = hsv[:, :, 1]
    v = hsv[:, :, 2]
    
    h60 = h / 60.0
    h60f = np.floor(h60)
    hi = np.remainder(np.int32(h60f), 6)
    f = h60 - h60f
    
    p = v * (1 - s)
    q = v * (1 - f * s)
    t = v * (1 - (1 - f) * s)
    
    r = np.zeros(hsv.shape[0 : 2])
    g = np.zeros(hsv.shape[0 : 2])
    b = np.zeros(hsv.shape[0 : 2])
        
    r[hi == 0] = v[hi == 0]
    g[hi == 0] = t[hi == 0]
    b[hi == 0] = p[hi == 0]
    
    r[hi == 1] = q[hi == 1]
    g[hi == 1] = v[hi == 1]
    b[hi == 1] = p[hi == 1]
    
    r[hi == 2] = p[hi == 2]
    g[hi == 2] = v[hi == 2]
    b[hi == 2] = t[hi == 2]
    
    r[hi == 3] = p[hi == 3]
    g[hi == 3] = q[hi == 3]
    b[hi == 3] = v[hi == 3]
        
    r[hi == 4] = t[hi == 4]
    g[hi == 4] = p[hi == 4]
    b[hi == 4] = v[hi == 4]
        
    r[hi == 5] = v[hi == 5]
    g[hi == 5] = p[hi == 5]
    b[hi == 5] = q[hi == 5]
    
    return np.concatenate((np.reshape(r, r.shape + (1,)),
                           np.reshape(g, g.shape + (1,)),
                           np.reshape(b, b.shape + (1,))), axis=2)

I = io.loadmat('Buddha.mat')
I = I['I']

fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(nrows=2, ncols=2)
ax1.imshow(I)

I_hsv = rgb2hsv(I)
V = I_hsv[:,:,2]

#%% linear scaling
# implement your method for linear scaling here:
I_linear = np.array(I_hsv)
s = 1./np.max(V)            # max(V) is 1 so this scaling doesn't change anything
V_linear = V * s
I_linear[:,:,2] = V_linear
I_linear = hsv2rgb(I_linear)

ax2.imshow(np.clip(I_linear, 0, 1))
#plt.show()

#%% gamma correction
I_gamma = np.array(I_hsv)
gamma = 1 / 4. # try correction with this gamma value

# implement your method for gamma correction here

ax3.imshow(np.clip(I_gamma, 0, 1))
#plt.show()

#%% histogram adjustment
I_hist = np.array(I)
nbins = 256 # number of bins

# use this box filtered image as an approximation of the 1 degree fov image
I_filtered = np.array(I)
boxfilter = np.ones((9, 9)) / (9 * 9)
for c in range(0, 3):
    I_filtered[:, :, c] = sig.convolve2d(I_filtered[:, :, c], boxfilter, mode='same', boundary='symm')

# implement your method for histogram scaling here
# hint: Take a look at the functions numpy.histogram(), numpy.cumsum() and numpy.interp()
# hint: You might want to perform the actual tonemapping by transforming the RGB
# image into the HSV color space and then only work on the V channel. Transform
# the tonemapped image back to RGB afterwards. For this you can use the two
# converter functions in the beginning of this script.

ax4.imshow(np.clip(I_hist, 0, 1))
plt.show()
