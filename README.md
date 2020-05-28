Building:

```bash
$ cmake . -DOPENMC=<PN/OFF> -DOPENMP=<ON/OFF>
$ make
```

CMake options:

  - openmc: Include support for rendering OpenMC scenes
  - openmp: Build with OpenMP parallelism

Usage:

```bash
$ ./tracer <scene_name> <image_width> <antialiasing_samples>
```

Scenes:

  - Native
    - three_spheres: series of three yellow spheres
    ![Three Spheres](https://github.com/pshriwise/openmc_render/blob/master/images/three_spheres.ppm)
    - book_cover: sene mimicking the cover of the weekend raytracing book
  - OpenMC (if enabled):
    - openmc_cover : scene mimicking the cover of the weekend raytracing book using OpenMC objects
    - openmc: renders a scene using OpenMC objects based on the local OpenMC model w/ randomly selected colors
