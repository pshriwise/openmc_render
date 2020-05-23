

import openmc
import numpy as np
from random import random

mat = openmc.Material()
mat.add_nuclide('U235', 1.0)
mat.set_density('g/cc', 11.0)

ground_sphere = openmc.Sphere(0, -1000, 0, 1000, boundary_type='vacuum')

ground = openmc.Cell(fill=mat, region=-ground_sphere, name="ground")

spheres = [ground]

middle = np.asarray((0, 0.2, 0))

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

        spheres.append(openmc.Cell(fill=mat, region=-sphere, name=cell_name))

# setup the three larger spheres
sphere1 = openmc.Sphere(0, 1, 0, 1.0, boundary_type='vacuum')
spheres.append(openmc.Cell(fill=mat, region=-sphere1, name='Dielectric'))

sphere2 = openmc.Sphere(-4, 1, 0, 1.0, boundary_type='vacuum')
spheres.append(openmc.Cell(fill=mat, region=-sphere2, name='Lambertian'))

sphere3 = openmc.Sphere(4, 1, 0, 1.0, boundary_type='vacuum')
spheres.append(openmc.Cell(fill=mat, region=-sphere3, name='Metal'))

model = openmc.model.Model()

model.geometry = openmc.Geometry(spheres)

model.materials = [mat]

model.settings.particles = 500
model.settings.batches = 10
model.settings.inactive = 1

model.export_to_xml()



