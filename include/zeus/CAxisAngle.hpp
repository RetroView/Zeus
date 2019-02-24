#pragma once

#include "Global.hpp"
#include "zeus/CVector3f.hpp"
#include "CUnitVector.hpp"

namespace zeus {
struct CAxisAngle : CVector3f {
  constexpr CAxisAngle() = default;
  constexpr CAxisAngle(float x, float y, float z) : CVector3f(x, y, z) {}
  CAxisAngle(const CUnitVector3f& axis, float angle) : CVector3f(angle * axis) {}
  constexpr CAxisAngle(const CVector3f& axisAngle) : CVector3f(axisAngle) {}
  float angle() const { return magnitude(); }
  const CVector3f& getVector() const { return *this; }
};
} // namespace zeus
