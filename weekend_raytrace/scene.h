
#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "geom.h"


struct Scene {
  ObjectList objects_;
  Camera camera_;

  inline Camera& camera() { return camera_; }
  inline ObjectList& objects() { return objects_; }

  // Hardcoded Scenes
  static Scene red_blue();
  static Scene three_spheres();
  static Scene book_cover();

  static Scene create(std::string scene_name) {
    if (scene_name == "red_blue") {
      return red_blue();
    } else if (scene_name == "three_spheres") {
      return three_spheres();
    } else if (scene_name == "book_cover") {
      return book_cover();
    } else {
      throw std::invalid_argument("Invalid scene name \"" + scene_name + "\" specified.");
    }
  }

};

#endif