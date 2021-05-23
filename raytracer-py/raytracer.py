#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

def normalize(vector):
    return vector / np.linalg.norm(vector)

width = 300
height = 200

# width = 1920
# height = 1080

camera = np.array([0, 0, 1])
ratio = float(width) / height
#        left,  top,   right,  bottom
screen = (-1, 1 / ratio, 1, -1 / ratio)

objects = [
    { 'center': np.array([-0.2, 0, -1]), 'radius': 0.7 },
    { 'center': np.array([0.1, -0.3, 0]), 'radius': 0.1 },
    { 'center': np.array([-0.3, 0, 0]), 'radius': 0.15 }
]

print("Camera: ", camera)
print("Ratio:  ", ratio)
print("Screen: ", screen)

i = 0
for obj in objects:
    i = i + 1
    print("Object ", i, ": ", obj)

image = np.zeros((height, width, 3))
for i, y in enumerate(np.linspace(screen[1], screen[3], height)):
    for j, x in enumerate(np.linspace(screen[0], screen[2], width)):
        pixel = np.array([x, y, 0])
        origin = camera
        direction = normalize(pixel - origin)
        # image[i, j] = ...
        # print("progress: %d/%d" % (i + 1, height))
        # pass

plt.imsave('image.png', image)
