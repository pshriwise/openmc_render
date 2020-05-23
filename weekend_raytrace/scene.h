
#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "geom.h"

#ifdef OPENMC
#include "openmc/surface.h"
#include "openmc/cell.h"
#endif

struct Scene {
  ObjectList objects_;
  Camera camera_;

  inline Camera& camera() { return camera_; }
  inline ObjectList& objects() { return objects_; }

};

// Hardcoded Scenes
Scene red_blue();
Scene three_spheres();
Scene book_cover();
#ifdef OPENMC
Scene openmc_setup();
#endif

Scene create_scene(std::string scene_name);

#endif