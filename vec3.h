

#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "rand.h"

using std::sqrt;

class Vec3 {
public:
  // Constructors
  Vec3() : e{0,0,0} {}
  Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  // Accessors
  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

  double operator[](int i) const { return e[i]; }
  double& operator[](int i) { return e[i]; }

  // Methods
  Vec3& operator+=(const Vec3 &v) {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];
      return *this;
  }

  Vec3& operator*=(const double t) {
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;
      return *this;
  }

  Vec3& operator/=(const double t) {
      return *this *= 1/t;
  }

  double length() const {
      return sqrt(length_squared());
  }

  double length_squared() const {
      return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
  }

  inline static Vec3 random() {
    return Vec3(nrand(), nrand(), nrand());
  }

  inline static Vec3 random(double min, double max) {
    return Vec3(nrand(min, max), nrand(min, max), nrand(min, max));
  }

  inline static Vec3 random_in_unit_sphere() {
    while (true) {
      auto p = Vec3::random(-1, 1);
      if (p.length_squared() >= 1) continue;
      return p;
    }
  }

public:
  // Data members
  double e[3];
};

// type aliases for Vec3
using Point3 = Vec3;

// Vec3 Utility Functions
inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3 &v) {
  return Vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vec3 operator*(const Vec3 &v, double t) {
  return t * v;
}

inline Vec3 operator/(Vec3 v, double t) {
  return (1/t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v) {
  return u.e[0] * v.e[0]
       + u.e[1] * v.e[1]
       + u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 unit_vector(Vec3 v) {
  return v / v.length();
}

#endif