

import openmc


pwr_ex = openmc.examples.pwr_pin_cell()

mats = pwr_ex.geometry.get_all_materials()

model = openmc.model.Model()

ground_sphere = openmc.Sphere(0, -100.5, 0, 100, boundary_type='vacuum')

cyl = openmc.YCylinder(0, -1, 0.5, boundary_type='vacuum')
ymax = openmc.YPlane(0.7, boundary_type='vacuum')
ymin = openmc.YPlane(0.1, boundary_type='vacuum')
top_cap = openmc.Sphere(0, 0.7, -1, 0.5, boundary_type='vacuum')
bottom_cap = openmc.Sphere(0, 0.1, -1, 0.5, boundary_type='vacuum')
ground_cell = openmc.Cell(fill=mats[1], region=-ground_sphere)

marble = openmc.Cell(fill=mats[3], region=(-cyl & -ymax & +ymin))
another_sphere = openmc.Sphere(1.0, 0.5, -1.0, 0.2, boundary_type='vacuum')

another_sphere_cell = openmc.Cell(fill=mats[1], region=-another_sphere)

model.geometry = openmc.Geometry([ground_cell, marble, another_sphere_cell])
model.materials = [mats[1], mats[3]]

model.settings.particles = 500
model.settings.batches = 10
model.settings.inactive = 1

model.export_to_xml()