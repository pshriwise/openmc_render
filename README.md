## Building

```bash
$ cmake . -DOPENMC=<PN/OFF> -DOPENMP=<ON/OFF>
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

A scene mimicking the cover of the "Raytracing in One Weekend" book using OpenMC objects.

  ![Book Cover](https://github.com/pshriwise/openmc_render/blob/master/images/openmc_book_cover.png)
#### openmc

 renders a scene using OpenMC objects based on the local OpenMC model w/ randomly selected colors
