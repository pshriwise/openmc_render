# OpenMC Rendering Pet Project

This project stemmed from working through the free book [Raytracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html) by Peter Shirley. The code presented in the book has been modified somewhat for a style I'm more familiar with and to allow for use of CSG representations from [OpenMC](https://github.com/openmc-dev/openmc/), an open source Monte Carlo radiation transport tool used at Argonne National Laboratory.

## Building

```bash
$ cmake . -DOPENMC=<ON/OFF> -DOPENMP=<ON/OFF>
$ make
```

## CMake options

  - openmc: Include support for rendering OpenMC scenes
  - openmp: Build with OpenMP parallelism

## Usage

```bash
$ ./tracer <scene_name> <image_width> <antialiasing_samples>
```

## Scenes

Any of the following scenes can be passed to the `tracer` program as the first argument.

### Native

#### three_spheres (default)

A row of three yellow spheres.

![Three Spheres](https://github.com/pshriwise/openmc_render/blob/master/images/three_spheres.png)

#### book_cover

A scene mimicking the cover of the "Raytracing in One Weekend" book.

![Book Cover](https://github.com/pshriwise/openmc_render/blob/master/images/book_cover.png)

### OpenMC (if enabled):
#### openmc_cover

A scene mimicking the cover of the "Raytracing in One Weekend" book using OpenMC objects. The
model created by the `openmc_book_cover.py` script in the models directory can be used to
generate the necessary OpenMC model.

  ![OpenMC Book Cover](https://github.com/pshriwise/openmc_render/blob/master/images/openmc_book_cover.png)
#### openmc

Renders a scene using OpenMC objects based on the local OpenMC model w/ randomly selected colors. The demo model
shown below can be generated using the `demo_model.py` script in the models directory.

  ![OpenMC General Demo](https://github.com/pshriwise/openmc_render/blob/master/images/openmc_demo.png)


### Camera Options

Camera parameters are all currently hardcoded for each scene in `src/scenes.cpp`.

