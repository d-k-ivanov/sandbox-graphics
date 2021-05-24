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


def reflected(vector, axis):
    return vector - 2 * np.dot(vector, axis) * axis


if __name__ == '__main__':
    # width = 300
    # height = 200
    # width = 800
    # height = 600
    # width = 1920
    # height = 1080
    width = 3840
    height = 2160

    max_depth = 3

    camera = np.array([0, 0, 1])
    ratio = float(width) / height
    #        left,  top,   right,  bottom
    screen = (-1, 1 / ratio, 1, -1 / ratio)

    light = { 'position': np.array([5, 5, 5]), 'ambient': np.array([1, 1, 1]), 'diffuse': np.array([1, 1, 1]), 'specular': np.array([1, 1, 1]) }

    # TODO: make more objects, let's say - glass spheres
    objects = [
        { 'center': np.array([-0.2,  0,  -1]), 'radius': 0.7,        'ambient': np.array([0.1, 0, 0]),     'diffuse': np.array([0.7, 0, 0]),     'specular': np.array([1, 1, 1]), 'shininess': 100, 'reflection': 0.5 },
        { 'center': np.array([ 0.1, -0.3, 0]), 'radius': 0.1,        'ambient': np.array([0.1, 0, 0.1]),   'diffuse': np.array([0.7, 0, 0.7]),   'specular': np.array([1, 1, 1]), 'shininess': 100, 'reflection': 0.5 },
        { 'center': np.array([-0.3,  0,   0]), 'radius': 0.15,       'ambient': np.array([0, 0.1, 0]),     'diffuse': np.array([0, 0.6, 0]),     'specular': np.array([1, 1, 1]), 'shininess': 100, 'reflection': 0.5 },
        { 'center': np.array([ 0, -9000, 0]),  'radius': 9000 - 0.7, 'ambient': np.array([0.1, 0.1, 0.1]), 'diffuse': np.array([0.6, 0.6, 0.6]), 'specular': np.array([1, 1, 1]), 'shininess': 100, 'reflection': 0.5 }
    ]

    image = np.zeros((height, width, 3))
    for i, y in enumerate(np.linspace(screen[1], screen[3], height)):
        for j, x in enumerate(np.linspace(screen[0], screen[2], width)):
            pixel = np.array([x, y, 0])
            origin = camera
            direction = normalize(pixel - origin)
            color = np.zeros((3))
            reflection = 1

            for k in range(max_depth):
                # check for intersections
                nearest_object, min_distance = nearest_intersected_object(objects, origin, direction)
                if nearest_object is None:
                    break

                # compute intersection point between ray and nearest object
                intersection = origin + min_distance * direction

                normal_to_surface = normalize(intersection - nearest_object['center'])
                shifted_point = intersection + 1e-5 * normal_to_surface
                intersection_to_light = normalize(light['position'] - intersection)

                _, min_distance = nearest_intersected_object(objects, intersection, intersection_to_light)
                intersection_to_light_distance = np.linalg.norm(light['position'] - intersection)
                is_shadowed = min_distance < intersection_to_light_distance

                if is_shadowed:
                    break

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

                # reflection
                color += reflection * illumination
                reflection *= nearest_object['reflection']

                # new ray origin and direction
                origin = shifted_point
                direction = reflected(direction, normal_to_surface)

            # Create image
            image[i, j] = np.clip(color, 0, 1)
            # print("progress: %d/%d" % (i + 1, height))
            # pass

    plt.imsave('image.png', image)

## TODO
# POO! Right now we’ve put all the objects in a dict, but you could make classes,
# figure out what’s specific to spheres and what’s not, make a base class, and implement other objects such as planes or triangles;
#
# Same thing goes for light. Add some POO here and make it so you can add multiple lights in the scene;
#
# Separate the material properties from the geometrical properties, to be able to apply one material (e.g. blue matte) to any type of objects;
#
# Figure out a way to position the screen correctly given any camera position and a direction to look at;
#
# Model the light differently. Currently it’s a single point, which is why the shadows of objects are “hard” or well defined.
# In order to get “soft” shadows (with a gradient basically), you need to model a light like a 2d or 3d object: disk or sphere?
