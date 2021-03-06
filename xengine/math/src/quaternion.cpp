#include "quaternion.h"

Quaternion& Quaternion::operator=(Quaternion&& b) noexcept
{
    x = b.x;
    y = b.y;
    z = b.z;
    w = b.w;
    return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& q)
{
    x *= q.x;
    y *= q.y;
    z *= q.z;
    w *= q.w;
    return *this;
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
    return Quaternion(y * q.z - z * q.y + q.x * w + x * q.w,
        z * q.x - x * q.z + q.y * w + y * q.w,
        x * q.y - y * q.x + q.z * w + z * q.w,
        w * q.w - (x * q.x + y * q.y + z * q.z));
}

Quaternion Quaternion::operator*(const real32 s) const
{
    return Quaternion(x * s, y * s, z * s, w * s);
}

Quaternion Quaternion::operator/(const Quaternion& q) const
{
    return Quaternion(x / q.x, y / q.y, z / q.z, w / q.w);
}

Quaternion& Quaternion::operator/=(const Quaternion& q)
{
    x /= q.x;
    y /= q.y;
    z /= q.z;
    w /= q.w;
    return *this;
}

Quaternion Quaternion::operator+(const Quaternion& rhs) const
{
    return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

Quaternion Quaternion::operator-(const Quaternion& rhs) const
{
    return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
}

void Quaternion::printString() const
{
    printf("Quaternion: %f %f %f %f\n", x, y, z, w);
}

bool Quaternion::operator==(Quaternion& b) const
{
    return x == b.x && y == b.y && z == b.z && w == b.w;
}

bool Quaternion::operator!=(const Quaternion& b) const
{
    return x != b.x && y != b.y && z != b.z && w != b.w;
}

void Quaternion::invert()
{
    x *= -1;
    y *= -1;
    z *= -1;
}

// Optimize
void Quaternion::normalize(real32 tolerance)
{
    real32 sqrSum = x * x + y * y + z * z + w * w;

    if (sqrSum >= tolerance)
    {
        const real32 scale = MathGeneral::InvSqrt(sqrSum);

        x *= scale;
        y *= scale;
        z *= scale;
        w *= scale;

    }
    else
        *this = Quaternion();
}

real32 Quaternion::length() const
{
    return x * x + y * y + z * z + w * w;
}

real32 Quaternion::lengthSqr() const
{
    return MathGeneral::Sqrt(length());
}

bool Quaternion::isNormalized() const
{
    return (Utils::abs(1.f - length()) < THRESH_QUAT_NORMALIZED);
}

Quaternion Quaternion::getInverted()
{
    if (!isNormalized())
        normalize();

    return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::getNormalized() const
{
    Quaternion quat;

    real32 len = length();

    quat.x /= len;
    quat.y /= len;
    quat.z /= len;
    quat.w /= len;

    return quat;
}


real32 Quaternion::dot(const Quaternion & a, const Quaternion & b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Quaternion Quaternion::nlerp(const Quaternion& q1, const Quaternion& q2, real32 t)
{
    return (q1 * (1.0f - t) + q2 * t).getNormalized();
}
