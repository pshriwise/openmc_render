import openmc


# Geometry
ground_sphere = openmc.Sphere(0, -100.5, 0, 100, boundary_type='vacuum')

cyl = openmc.YCylinder(0, -1, 0.5, boundary_type='vacuum')
ymax = openmc.YPlane(1.0, boundary_type='vacuum')
ymin = openmc.YPlane(0.3, boundary_type='vacuum')
top_cap = openmc.Sphere(0, 1.0, -1, 0.5, boundary_type='vacuum')
bottom_cap = openmc.Sphere(0, 0.3, -1, 0.5, boundary_type='vacuum')
ground_cell = openmc.Cell(region=-ground_sphere)

pill = openmc.Cell(region=((-cyl & -ymax & +ymin) | -top_cap | -bottom_cap))
another_sphere = openmc.Sphere(-1.5, 0.5, -1.0, 0.5, boundary_type='vacuum')
another_sphere_cell = openmc.Cell(region=-another_sphere)

ycone = openmc.YCone(1.5, 1.0, -1, 0.5, boundary_type='vacuum')
cone_cell = openmc.Cell(region=(-ycone & -ymax & +ymin))

geometry = openmc.Geometry([ground_cell, pill, another_sphere_cell, cone_cell])
geometry.export_to_xml()

# Placeholder materials file
openmc.Materials().export_to_xml()

# Minimal settings
settings = openmc.Settings()
settings.particles = 500
settings.batches = 10
settings.inactive = 1

settings.export_to_xml()