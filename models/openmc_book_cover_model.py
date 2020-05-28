

import openmc
import numpy as np
from random import random

ground_sphere = openmc.Sphere(0, -1000, 0, 1000, boundary_type='vacuum')

ground = openmc.Cell(region=-ground_sphere, name="ground")

spheres = [ground]

middle = np.asarray((0, 0.2, 0))

# randomly placed spherical cells
for a in range(-11, 11):
    for b in range(-11, 11):
        center = np.asarray((a + 0.9 * random(), 0.2, b + 0.9 * random()))

        if (np.linalg.norm(middle - center) < 3.0):
            continue

        sphere = openmc.Sphere(*center, 0.2, boundary_type='vacuum')

        mat_rand = random()
        if mat_rand < 0.8:
            cell_name = "Lambertian"
        elif mat_rand < 0.95:
            cell_name = "Metal"
        else:
            cell_name = "Dielectric"

        spheres.append(openmc.Cell(region=-sphere, name=cell_name))

# setup the three larger spheres
sphere1 = openmc.Sphere(0, 1, 0, 1.0, boundary_type='vacuum')
spheres.append(openmc.Cell(region=-sphere1, name='Dielectric'))

sphere2 = openmc.Sphere(-4, 1, 0, 1.0, boundary_type='vacuum')
spheres.append(openmc.Cell(region=-sphere2, name='Lambertian'))

sphere3 = openmc.Sphere(4, 1, 0, 1.0, boundary_type='vacuum')
spheres.append(openmc.Cell(region=-sphere3, name='Metal'))

model = openmc.model.Model()

geometry = openmc.Geometry(spheres)
geometry.export_to_xml()

# Placeholder materials file
openmc.Materials().export_to_xml()

settings = openmc.Settings()
settings.particles = 500
settings.batches = 10
settings.inactive = 1

settings.export_to_xml()



