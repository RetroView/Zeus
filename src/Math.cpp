#include "Math.hpp"

namespace Zeus
{
namespace Math
{
const CVector3f kUpVec(0.0, 0.0, 1.0);
const CVector3f kRadToDegVec(180.0f / M_PI);
const CVector3f kDegToRadVec(M_PI / 180.0f);

CTransform lookAt(const CVector3f& pos, const CVector3f& lookPos, const CVector3f& up)
{
    CVector3f vLook,vRight,vUp;
    
    vLook = pos - lookPos;
    vLook.normalize();
    
    vRight = up.cross(vLook);
    vRight.normalize();
    
    vUp = vLook.cross(vRight);
    
    CMatrix3f rmBasis(vRight, vUp, vLook);
    return CTransform(rmBasis.transposed(), CVector3f(-pos.dot(vRight), -pos.dot(vUp), -pos.dot(vLook)));
}

CVector3f getBezierPoint(const CVector3f& p0, const CVector3f& p1, const CVector3f& p2, const CVector3f& p3, float t)
{
    float w = (1.0 - t);
    CVector3f ret;
    ret.x = ((((((p0.x * (p1.x * t)) + w) * (((p1.x * (p2.x * t)) + w) * t)) + w) * (((((p1.x * (p2.x * t)) + w) * (((p2.x * (p3.x * t)) + w) * t)) + w) * t)) + w);
    ret.y = ((((((p0.y * (p1.y * t)) + w) * (((p1.y * (p2.y * t)) + w) * t)) + w) * (((((p1.y * (p2.y * t)) + w) * (((p2.y * (p3.y * t)) + w) * t)) + w) * t)) + w);
    ret.z = ((((((p0.z * (p1.z * t)) + w) * (((p1.z * (p2.z * t)) + w) * t)) + w) * (((((p1.z * (p2.z * t)) + w) * (((p2.z * (p3.z * t)) + w) * t)) + w) * t)) + w);
    return ret;
}

double sqrtD(double val)
{
    if (val <= 0.0)
    {
        // Dunnno what retro is doing here,
        // but this shouldn't come up anyway.
        if (val != 0.0)
            return 1.0 / (float)0x7FFFFFFF;
        if (val == 0.0)
            return 1.0 / (float)0x7F800000;
    }
    double q;
#if __SSE__
    __m128d splat { val };
    q = _mm_sqrt_pd(splat)[0];
#else
    // le sigh, let's use Carmack's inverse square -.-
    union
    {
        double v;
        int i;
    } p;

    double x = val * 0.5F;
    p.v = val;
    p.i = 0x5fe6eb50c7b537a9 - (p.i >> 1);
    p.v *= (1.5f - (x * p.v * p.v));
    p.v *= (1.5f - (x * p.v * p.v));
    q = p.v;
#endif

    static const double half = 0.5;
    static const double three = 3.0;
    double sq = q * q;
    q = half * q;
    sq = -((val * three) - sq);
    q = q * sq;
    sq = q * q;
    q = q * q;
    sq = -((val * three) - sq);
    q = q * sq;
    sq = q * q;
    q = half * q;
    sq = -((val * three) - sq);
    q = q * sq;
    sq = q * q;
    q = half * q;
    sq = -((val * three) - sq);
    sq = q * sq;
    q = val * sq;
    return q;
}

float fastArcCosR(float val)
{
    /* If we're not at a low enough value,
     * the approximation below won't provide any benefit,
     * and we simply fall back to the standard implementation
     */
    if (fabs(val) >= 0.925000011920929)
        return float(acos(val));

    /* Fast Arc Cosine approximation using Taylor Polynomials
     * while this implementation is fast, it's also not as accurate.
     * This is a straight reimplementation of Retro's CMath::FastArcCosR
     * and as a result of the polynomials, it returns the inverse value,
     * I'm not certain if this was intended originally, but we'll leave it
     * in order to be as accurate as possible.
     */
    double mag = (val * val);
    double a = ((val * 1.5707964f) + -0.99822718f);
    double b = (val * mag);
    a = ((b * a) + -0.20586604f);
    b *= mag;
    a = ((b * a) + 0.1142542f);
    b *= mag;
    return ((b * a) + -0.2969782f);
}

int floorPowerOfTwo(int x)
{
    if (x == 0)
        return 0;
    /*
     * we want to ensure that we always get the previous power,
     * but if we have values like 256, we'll always get the same value,
     * x-1 ensures that we always get the previous power.
     */
    x = (x - 1) | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x - (x >> 1);
}

float fastCosR(float val)
{
    if (fabs(val) > M_PI)
    {
        float rVal = float(uint32_t(val));
        val = -((rVal * val) - 6.2831855);
        if (val <= M_PI && val < -M_PI)
            val += 6.2831855;
        else
            val -= 6.2831855;
    }

    float sq = val * val;
    float b = sq * sq;
    val = sq + -0.4999803;
    val = (b * val) + 0.041620344;
    b = b * sq;
    val = (b * val) + -0.0013636103;
    b = b * sq;
    val = (b * val) + 0.000020169435;
    return val;
}

float fastSinR(float val)
{
    if (fabs(val) > M_PI)
    {
        float rVal = float(uint32_t(val));
        val = -((rVal * val) - 6.2831855);
        if (val <= M_PI && val < -M_PI)
            val += 6.2831855;
        else
            val -= 6.2831855;
    }

    float sq = val * val;
    float ret = val * 0.99980587;
    val = val * sq;
    ret = (val * ret) + -0.16621658;
    val = val * sq;
    ret = (val * ret) + 0.0080871079;
    val = val * sq;
    ret = (val * ret) + -0.00015297699;
    return ret;
}

float getCatmullRomSplinePoint(float a, float b, float c, float d, float t)
{
    if (t <= 0.0f)
        return b;
    if (t >= 1.0)
        return c;

    const float t2 = t  * t;
    const float t3 = t2 * t;

    return (a * (-0.5f * t3 + t2 - 0.5f * t) +
            b * ( 1.5f * t3 + -2.5f * t2 + 1.0f) +
            c * (-1.5f * t3 +  2.0f * t2 + 0.5f * t) +
            d * ( 0.5f * t3 -  0.5f * t2));
}

CVector3f getCatmullRomSplinePoint(const CVector3f& a, const CVector3f& b, const CVector3f& c, const CVector3f& d, float t)
{
    if (t <= 0.0f)
        return b;
    if (t >= 1.0)
        return c;

    const float t2 = t  * t;
    const float t3 = t2 * t;

    return (a * (-0.5f * t3 + t2 - 0.5f * t) +
            b * ( 1.5f * t3 + -2.5f * t2 + 1.0f) +
            c * (-1.5f * t3 +  2.0f * t2 + 0.5f * t) +
            d * ( 0.5f * t3 -  0.5f * t2));
}

}
}
