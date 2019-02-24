#include "zeus/CVector3f.hpp"
#include "zeus/CVector3d.hpp"
#include <memory.h>
#include <cmath>
#include <cassert>
#include "zeus/Math.hpp"
#include "zeus/CRelAngle.hpp"

namespace zeus {
float CVector3f::getAngleDiff(const CVector3f& a, const CVector3f& b) {
  float mag1 = a.magnitude();
  float mag2 = b.magnitude();

  if (!mag1 || !mag2)
    return 0.f;

  float dot = a.dot(b);
  float theta = std::acos(dot / (mag1 * mag2));
  return theta;
}

CVector3f CVector3f::slerp(const CVector3f& a, const CVector3f& b, CRelAngle clampAngle) {
  return a * std::cos(clampAngle) + a.cross(b).normalized().cross(a) * std::sin(clampAngle);
}
} // namespace zeus
