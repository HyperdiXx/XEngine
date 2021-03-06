#include "mat4.h"

    mat4 mat4::operator*(const mat4& rhs) const 
    {
        mat4 res = mat4::identity();
        for (int i = 0; i < ROW_COUNT; ++i) 
        {
            for (int j = 0; j < ROW_COUNT; ++j)
            {
                for (int k = 0; k < ROW_COUNT; ++k) 
                {
                   // res.row_col[i][j] += row_col[i][k] * rhs.row_col[k][j];
                }
            }
        }
        return res;
    }

    mat4 mat4::translate(const vec3f &tran)
    {
        mat4 res(1.0f);

        res.elem[0 + 3 * 4] = tran.x;
        res.elem[1 + 3 * 4] = tran.y;
        res.elem[2 + 3 * 4] = tran.z;

        return (res);
    }

    mat4 mat4::rotate(real32 angle, const vec3f & axis)
    {
        mat4 res(1.0f);

        real32 r = Utils::toRadians(angle);
        real32 c = cos(r);
        real32 s = sin(r);

        real32 x = axis.x;
        real32 y = axis.y;
        real32 z = axis.z;
        real32 o = 1.0f - c;

        res.elem[0 + 0 * 4] = x * o + c;
        res.elem[1 + 0 * 4] = y * x * o + z * s;
        res.elem[2 + 0 * 4] = x * z * o - y * s;

        res.elem[0 + 1 * 4] = x * y * o - z * c;
        res.elem[1 + 1 * 4] = y * o + c;
        res.elem[2 + 1 * 4] = y * z * o + x * s;

        res.elem[0 + 2 * 4] = x * z * o + y * s;
        res.elem[1 + 2 * 4] = y * z * o - x * s;
        res.elem[2 + 2 * 4] = z * o + c;

        return (res);
    }

    mat4 mat4::scale(const vec3f& scale)
    {
        mat4 res(1.0f);

        res.elem[0 + 0 * 4] = scale.x;
        res.elem[1 + 1 * 4] = scale.y;
        res.elem[2 + 2 * 4] = scale.z;

        return (res);
    }

    mat4 mat4::rotateRes(const vec3f & rotationVec)
    {
        return rotateX(rotationVec.x) * rotateY(rotationVec.y) * rotateZ(rotationVec.z);
    }

    mat4 mat4::rotateX(real32 radians)
    {
        mat4 res = mat4::identity();

        real32 cos = MathGeneral::Cos(radians);
        real32 sin = MathGeneral::Sin(radians);

        res.m22 = cos;
        res.m23 = sin;
        res.m32 = -sin;
        res.m33 = cos;

        return (res);
    }

    mat4 mat4::rotateY(real32 radians)
    {
        mat4 res = mat4::identity();

        real32 cos = MathGeneral::Cos(radians);
        real32 sin = MathGeneral::Sin(radians);

        res.m11 = cos;
        res.m13 = -sin;
        res.m31 = sin;
        res.m33 = cos;

        return (res);
    }

    mat4 mat4::rotateZ(real32 radians)
    {
        mat4 res = mat4::identity();

        real32 cos = MathGeneral::Cos(radians);
        real32 sin = MathGeneral::Sin(radians);

        res.m11 = cos;
        res.m12 = sin;
        res.m21 = -sin;
        res.m22 = cos;

        return (res);
    }

    mat4 mat4::transform(const vec3f& matt, const vec3f& matr, const vec3f& mats)
    {
        return translate(matt) * rotateRes(matr) * scale(mats);
    }
    
    inline mat4 mat4::invert() const
    {
        return mat4();
    }

    inline mat4 mat4::getInverted()
    {
        return mat4();
    }

    real32 mat4::determinant()
    {
        return real32();
    }

    bool mat4::isZero() const
    {
        for (int i = 0; i < ELEM_COUNT; ++i) 
        {
            if (elem[i] != 0) 
                return false;
        }

        return true;
    }

    bool mat4::isIdentity() const
    {
        if (elem[0 + 0 * ROW_COUNT] == 1.0f && elem[1 + 1 * ROW_COUNT] == 1.0f && elem[2 + 2 * ROW_COUNT] == 1.0f)
            return true;

        return false;
    }

    void mat4::setDiagonal(const vec4f& diagonal)
    {
        setDiagonal(diagonal.x, diagonal.y, diagonal.z, diagonal.z);
    }

    void mat4::setDiagonal(real32 m1, real32 m2, real32 m3)
    {
        setDiagonal(m1, m2, m3, 1.0f);
    }

    void mat4::setDiagonal(real32 r0, real32 r1, real32 r2, real32 r3)
    {
        m00 = r0;
        m11 = r1;
        m22 = r2;
        m33 = r3;
    }

    real32 mat4::operator[](int i) const
    {
        if(i > 0 || i < ELEM_COUNT - 1)
            return elem[i];
        return 0.0f;
    }

    bool mat4::operator==(const mat4 & r) const
    {
        for (int i = 0; i < ELEM_COUNT; ++i)
        {
            if (elem[i] != r.elem[i])
                return false;
        }

        return true;
    }

    mat4& mat4::operator=(const mat4 & rhs)
    {
        for (size_t i = 0; i < ELEM_COUNT; ++i)
            elem[i] = rhs.elem[i];
        return *this;
    }

    bool mat4::operator!=(const mat4 & r) const
    {
        for (int i = 0; i < ELEM_COUNT; ++i)
        {
            if (elem[i] != r.elem[i])
            {
                return true;
            }
        }
        return false;
    }

    mat4 mat4::operator+(const mat4 & r) const
    {
        mat4 res;

        for (int i = 0; i < ELEM_COUNT; ++i)
        {
            res.elem[i] += r.elem[i];
        }

        return res;
    }

    mat4 & mat4::operator+=(const mat4 & r)
    {
        for (int i = 0; i < ELEM_COUNT; ++i)
            elem[i] += r.elem[i];

        return *this;
    }

    mat4 mat4::operator-(const mat4 & r) const
    {
        mat4 res;

        for (int i = 0; i < ELEM_COUNT; ++i)
        {
            res.elem[i] -= r.elem[i];
        }

        return res;
    }

    mat4 mat4::operator-=(const mat4 & r)
    {
        for (int i = 0; i < ELEM_COUNT; ++i)
        {
            elem[i] -= r.elem[i];
        }
        return *this;
    }


    mat4 mat4::operator*(const real32 scalar) const
    {
        mat4 res;

        for (int i = 0; i < ELEM_COUNT; i++)
        {
            res.elem[i] = elem[i] * scalar;
        }

        return res;
    }

    mat4 mat4::operator*=(const real32 scalar)
    {
        for (int i = 0; i < ELEM_COUNT; i++)
        {
            elem[i] *= scalar;
        }

        return *this;
    }

    /*vec4f mat4::operator*(const vec4f& r)
    {
        vec4f res;

        res.x = elem[0] * r.x + elem[1] * r.y + elem[2] * r.z + elem[3] * r.w;
        res.y = elem[4] * r.x + elem[5] * r.y + elem[6] * r.z + elem[7] * r.w;
        res.z = elem[8] * r.x + elem[9] * r.y + elem[10] * r.z + elem[11] * r.w;
        res.w = elem[12] * r.x + elem[13] * r.y + elem[14] * r.z + elem[15] * r.w;
            
        return res;
    }*/

    mat4 mat4::ortho(real32 left, real32 right, real32 top, real32 bottom, real32 nearplane, real32 farplane)
    {
        mat4 res(1.0f);

        res.elem[0 + 0 * ROW_COUNT] = 2.0f / (right - left);
        res.elem[1 + 1 * ROW_COUNT] = 2.0f / (top - bottom);
        res.elem[2 + 2 * ROW_COUNT] = 2.0f / (nearplane - farplane);

        res.elem[0 + 3 * ROW_COUNT] = (left + right) / (left - right);
        res.elem[1 + 3 * ROW_COUNT] = (bottom + top) / (top - bottom);
        res.elem[2 + 3 * ROW_COUNT] = (farplane + nearplane) / (farplane - nearplane);

        return res;
    }

    mat4 mat4::perspective(real32 fov, real32 aspectratio, real32 nearplane, real32 farplane)
    {
        mat4 res;

        real32 q = 1.0f / tan(Utils::toRadians(0.5f * fov));
        real32 a = q / aspectratio;
        real32 b = (nearplane + farplane) / (nearplane - farplane);
        real32 c = (2.0f  * nearplane * farplane) / (nearplane - farplane);
        res.elem[0 + 0 * ROW_COUNT] = a;
        res.elem[1 + 1 * ROW_COUNT] = q;
        res.elem[2 + 2 * ROW_COUNT] = b;
        res.elem[3 + 2 * ROW_COUNT] = -1.0f;
        res.elem[2 + 3 * ROW_COUNT] = c;

        return res;
    }

    void mat4::set(int32 i, real32 val)
    {
        if (i > 0 || i < ELEM_COUNT - 1)
            elem[i] = val;
    }

    void mat4::set(int32 i, int32 j, real32 val)
    {
        int pos = i + j * ROW_COUNT;

        if (pos > 0 || pos < ELEM_COUNT - 1)
            elem[pos] = val;
    }

    real32 mat4::get(int32 i) const
    {
        if(i > 0 || i < ELEM_COUNT - 1)
            return elem[i];
        return 0.0f;
    }

    int32 mat4::getIndex(int32 val) const 
    {
        for (int i = 0; i < ELEM_COUNT; ++i)
        {
            if (elem[i] == val)
                return i;
        }

        return false;
    }

    inline void mat4::zero()
    {
        for (int i = 0; i < ELEM_COUNT; i++)
            elem[i] = 0.0f;
    }

    inline void mat4::transpose()
    {        
        mat4 res = *this;

        m00 = res.m00;
        m01 = res.m10;
        m02 = res.m20;
        m03 = res.m30;
        m10 = res.m01;
        m11 = res.m11;
        m12 = res.m21;
        m13 = res.m31;
        m20 = res.m02;
        m21 = res.m12;
        m22 = res.m22;
        m23 = res.m32;
        m30 = res.m03;
        m31 = res.m13;
        m32 = res.m23;
        m33 = res.m33;
    }

    inline mat4 mat4::getTransposed()
    {
        mat4 res(1.0f);
        res.transpose();
        return res;
    }

    real32 mat4::get(int32 i, int32 j) const
    {
        int pos = i + j * ROW_COUNT;

        if(pos > 0 || pos < ELEM_COUNT - 1)
            return elem[pos];

        return false;
    }

    std::string mat4::toString() const
    {
        std::string res;
        return res;
    }
