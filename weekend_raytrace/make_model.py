

import openmc


pwr_ex = openmc.examples.pwr_pin_cell()

mats = pwr_ex.geometry.get_all_materials()

model = openmc.model.Model()

#ground_sphere = openmc.Sphere(0, -100.5, 0, 100, boundary_type='vacuum')

cyl = openmc.YCylinder(0, -1, 0.5, boundary_type='vacuum')
ymax = openmc.YPlane(0.1, boundary_type='vacuum')
ymin = openmc.YPlane(0.0, boundary_type='vacuum')

#ground_cell = openmc.Cell(fill=mats[1], region=-ground_sphere)

marble = openmc.Cell(fill=mats[3], region=-cyl & -ymax & +ymin)

model.geometry = openmc.Geometry([marble])
model.materials = [mats[1], mats[3]]

model.settings.particles = 500
model.settings.batches = 10
model.settings.inactive = 1


model.export_to_xml()

