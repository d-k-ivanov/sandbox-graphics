#!/usr/bin/env python
# -*- coding: utf-8 -*-

# for each pixel p(x,y,z) of the screen:
#     associate a black color to p
#     if the ray (line) that starts at camera and goes towards p intersects any object of the scene then:
#         calculate the intersection point to the nearest object
#         if there is no object of the scene in-between the intersection point and the light then:
#             calculate the color of the intersection point
#             associate the color of the intersection point to p

import numpy as np
import matplotlib.pyplot as plt


def normalize(vector):
    return vector / np.linalg.norm(vector)


def sphere_intersect(center, radius, ray_origin, ray_direction):
    b = 2 * np.dot(ray_direction, ray_origin - center)
    c = np.linalg.norm(ray_origin - center) ** 2 - radius ** 2
    delta = b ** 2 - 4 * c
    if delta > 0:
        t1 = (-b + np.sqrt(delta)) / 2
        t2 = (-b - np.sqrt(delta)) / 2
        if t1 > 0 and t2 > 0:
            return min(t1, t2)
    return None


def nearest_intersected_object(objects, ray_origin, ray_direction):
    distances = [sphere_intersect(obj['center'], obj['radius'], ray_origin, ray_direction) for obj in objects]
    nearest_object = None
    min_distance = np.inf
    for index, distance in enumerate(distances):
        if distance and distance < min_distance:
            min_distance = distance
            nearest_object = objects[index]
    return nearest_object, min_distance


if __name__ == '__main__':
    # width = 300
    # height = 200
    width = 1920
    height = 1080

    camera = np.array([0, 0, 1])
    ratio = float(width) / height
    #        left,  top,   right,  bottom
    screen = (-1, 1 / ratio, 1, -1 / ratio)

    light = { 'position': np.array([5, 5, 5]), 'ambient': np.array([1, 1, 1]), 'diffuse': np.array([1, 1, 1]), 'specular': np.array([1, 1, 1]) }

    objects = [
        { 'center': np.array([-0.2,  0,  -1]), 'radius': 0.7,  'ambient': np.array([0.1, 0, 0]),   'diffuse': np.array([0.7, 0, 0]),   'specular': np.array([1, 1, 1]), 'shininess': 100 },
        { 'center': np.array([ 0.1, -0.3, 0]), 'radius': 0.1,  'ambient': np.array([0.1, 0, 0.1]), 'diffuse': np.array([0.7, 0, 0.7]), 'specular': np.array([1, 1, 1]), 'shininess': 100 },
        { 'center': np.array([-0.3,  0,   0]), 'radius': 0.15, 'ambient': np.array([0, 0.1, 0]),   'diffuse': np.array([0, 0.6, 0]),   'specular': np.array([1, 1, 1]), 'shininess': 100 }
    ]

    image = np.zeros((height, width, 3))
    for i, y in enumerate(np.linspace(screen[1], screen[3], height)):
        for j, x in enumerate(np.linspace(screen[0], screen[2], width)):
            pixel = np.array([x, y, 0])
            origin = camera
            direction = normalize(pixel - origin)

            # check for intersections
            nearest_object, min_distance = nearest_intersected_object(objects, origin, direction)
            if nearest_object is None:
                continue

            # compute intersection point between ray and nearest object
            intersection = origin + min_distance * direction

            normal_to_surface = normalize(intersection - nearest_object['center'])
            shifted_point = intersection + 1e-5 * normal_to_surface
            intersection_to_light = normalize(light['position'] - intersection)

            _, min_distance = nearest_intersected_object(objects, intersection, intersection_to_light)
            intersection_to_light_distance = np.linalg.norm(light['position'] - intersection)
            is_shadowed = min_distance < intersection_to_light_distance

            if is_shadowed:
                continue

            # The Blinn-Phong model
            # https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
            # https://en.wikipedia.org/wiki/Phong_reflection_model
            # RGB
            illumination = np.zeros((3))

            # ambiant
            illumination += nearest_object['ambient'] * light['ambient']

            # diffuse
            illumination += nearest_object['diffuse'] * light['diffuse'] * np.dot(intersection_to_light, normal_to_surface)

            # specular
            intersection_to_camera = normalize(camera - intersection)
            H = normalize(intersection_to_light + intersection_to_camera)
            illumination += nearest_object['specular'] * light['specular'] * np.dot(normal_to_surface, H) ** (nearest_object['shininess'] / 4)

            image[i, j] = np.clip(illumination, 0, 1)
            # print("progress: %d/%d" % (i + 1, height))
            # pass

    plt.imsave('image.png', image)
