#ifndef CQUATERNION_HPP
#define CQUATERNION_HPP

#include "Global.hpp"
#include "CAxisAngle.hpp"
#include "CVector3f.hpp"
#include "CVector4f.hpp"
#include <math.h>
#if ZE_ATHENA_TYPES
#include <Athena/IStreamReader.hpp>
#endif

namespace Zeus
{
class alignas(16) CQuaternion
{
#if __atdna__ && ZE_ATHENA_TYPES
    float clangVec __attribute__((__vector_size__(16)));
#endif
public:
    ZE_DECLARE_ALIGNED_ALLOCATOR();
    
    CQuaternion() : r(1.0f) {}
    CQuaternion(float r, float x, float y, float z) : v(x, y, z){ this->r = r; }
    CQuaternion(float x, float y, float z) { fromVector3f(CVector3f(x, y, z)); }
    CQuaternion(float r, const CVector3f& vec) : v(vec){ this->r = r;}
#if ZE_ATHENA_TYPES
    CQuaternion(Athena::io::IStreamReader& input) { r = input.readFloat(); v = CVector3f(input);}
    CQuaternion(const atVec4f& vec)
    {
#if __SSE__
        v.mVec128 = vec.mVec128;
#else
        x = vec.vec[0]; y = vec.vec[1]; z = vec.vec[2]; r = vec.vec[3];
#endif
    }

    operator atVec4f()
    {
        atVec4f ret;
#if __SSE__
        ret.mVec128 = v.mVec128;
#else
        ret.vec = v;
#endif
        return ret;
    }
    operator atVec4f() const
    {
        atVec4f ret;
#if __SSE__
        ret.mVec128 = v.mVec128;
#else
        ret.vec = v;
#endif
        return ret;
    }

#endif
    CQuaternion(const CVector3f& vec) { fromVector3f(vec); }
    CQuaternion(const CVector4f& vec)
    {
#if __SSE__
        v.mVec128 = vec.mVec128;
#else
        v.x = vec.x; v.y = vec.y; v.z = vec.z; r = vec.w;
#endif
    }

    virtual ~CQuaternion() {}

    void fromVector3f(const CVector3f& vec);

    CQuaternion& operator=(const CQuaternion& q);
    CQuaternion operator+(const CQuaternion& q) const;
    CQuaternion operator-(const CQuaternion& q) const;
    CQuaternion operator*(const CQuaternion& q) const;
    CQuaternion operator/(const CQuaternion& q) const;
    CQuaternion operator*(float scale) const;
    CQuaternion operator/(float scale) const;
    CQuaternion operator-() const;
    const CQuaternion& operator+=(const CQuaternion& q);
    const CQuaternion& operator-=(const CQuaternion& q);
    const CQuaternion& operator*=(const CQuaternion& q);
    const CQuaternion& operator*=(float scale);
    const CQuaternion& operator/=(float scale);
    float magnitude() const;
    float magSquared() const;
    void normalize();
    CQuaternion normalized() const;
    void invert();
    CQuaternion inverse() const;

    /**
     * @brief Set the rotation using axis angle notation
     * @param axis The axis to rotate around
     * @param angle The magnitude of the rotation in radians
     * @return
     */
    static inline CQuaternion fromAxisAngle(const CVector3f& axis, float angle)
    {
        return CQuaternion(cosf(angle/2), axis*sinf(angle/2));
    }

    void rotateX(float angle) { *this *= fromAxisAngle({1.0f, 0.0f, 0.0f}, angle); }
    void rotateY(float angle) { *this *= fromAxisAngle({0.0f, 1.0f, 0.0f}, angle); }
    void rotateZ(float angle) { *this *= fromAxisAngle({0.0f, 0.0f, 1.0f}, angle); }


    CAxisAngle toAxisAngle();

    static inline CVector3f rotate(const CQuaternion& rotation, const CVector3f& v)
    {
        CQuaternion q = rotation * v;
        q *= rotation.inverse();

        return q.v;
    }

    CQuaternion log() const;

    CQuaternion exp() const;

    float dot(const CQuaternion& quat);

    static CQuaternion lerp(CQuaternion& a, CQuaternion& b, double t);
    static CQuaternion slerp(CQuaternion& a, CQuaternion& b, double t);
    static CQuaternion nlerp(CQuaternion& a, CQuaternion& b, double t);
    
    inline float roll() const
    {
        return atan2f(2.0 * (v.x * v.y + r * v.z), r * r + v.x * v.x - v.y * v.y - v.z * v.z);
    }
    
    inline float pitch() const
    {
        return atan2f(2.0 * (v.y * v.z + r * v.x), r * r - v.x * v.x - v.y * v.y + v.z * v.z);
    }
    
    inline float yaw() const
    {
        return asinf(-2.0 * (v.x * v.z - r * v.y));
    }

    union
    {
        struct { float x, y, z, r; };
        CVector3f v;
    };
};

CQuaternion operator+(float lhs, const CQuaternion& rhs);
CQuaternion operator-(float lhs, const CQuaternion& rhs);
CQuaternion operator*(float lhs, const CQuaternion& rhs);
}
#endif // CQUATERNION_HPP
