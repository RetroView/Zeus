#ifndef CFRUSTUM_HPP
#define CFRUSTUM_HPP

#include "zeus/CPlane.hpp"
#include "zeus/CAABox.hpp"
#include "zeus/CProjection.hpp"

namespace zeus
{
class CFrustum
{
    CPlane planes[6];
    bool valid = false;

public:
    void updatePlanes(const CMatrix4f& viewMtx, const CMatrix4f& projection);
    void updatePlanes(const CTransform& viewPointMtx, const CProjection& projection);

    bool aabbFrustumTest(const CAABox& aabb) const;
    bool sphereFrustumTest(const CSphere& sphere) const;
    bool pointFrustumTest(const CVector3f& point) const;
};
}
#endif // CFRUSTUM_HPP
