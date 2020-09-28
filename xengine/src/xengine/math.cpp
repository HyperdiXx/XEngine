
#include <math.h>

#define PI 					3.1415926535897932f	
#define SMALL_NUMBER		1.e-8f
#define DELTA			    0.00001f
#define EPSILON             0.00000001f

#define RadiansToDegrees(angle) angle * (180.0f / PI)
#define DegreesToRadians(angle) angle * (PI / 180.0f)

typedef float Degree;
typedef float Radian;

struct Vec2
{
    union
    {
        struct
        {
            real32 x, y;
        };
        real32 data[2];
    };
};

inline real32 LengthSqr(Vec2 vec)
{
    return vec.x * vec.x + vec.y + vec.y;
}

inline real32 Length(Vec2 vec)
{
    return sqrtf(LengthSqr(vec));
}

inline real32 DotProduct(Vec2 v1, Vec2 v2)
{
    return v1.x * v2.x + v1.y + v2.y;
}

inline real32 CrossProduct(Vec2 v1, Vec2 v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

inline real32 FastInverseSqrt(real32 lSqr)
{
    const real32 threeHalfs = 1.5f;

    real32 x2 = lSqr * 0.5f;
    real32 y = lSqr;
    uint64 i = *(uint64 *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(real32 *)&i;
    y = y * (threeHalfs - (x2 * y * y));

    return y;
}

inline Vec2 Normalize(Vec2 vec)
{
    Vec2 result = {};

    real32 lengthSqr = LengthSqr(vec);
    real32 invSqr = FastInverseSqrt(lengthSqr);
    if (invSqr > 0)
    {
        result.x *= invSqr;
        result.y *= invSqr;
    }

    return result;
}

inline real32 Determinant(Vec2 v1, Vec2 v2)
{
    return CrossProduct(Normalize(v1), Normalize(v2));
}

inline Vec2 Round(Vec2 v1)
{
    return { roundf(v1.x), roundf(v1.y) };
}

inline real32 Lerp(real32 t, real32 x, real32 y)
{
    return x + t * (y - x);
}

inline Vec2 Lerp(real32 t, Vec2 min, Vec2 max)
{
    Vec2 result = {};
    
    result.x = Lerp(t, min.x, max.x);
    result.y = Lerp(t, min.y, max.y);
    
    return result;
}

inline Vec2 NormalLine(Vec2 v1, Vec2 v2)
{
    real32 diffX = v2.x - v1.x;
    real32 diffY = v2.y - v1.y;

    Vec2 result = {};

    result.x = -diffY;
    result.y = diffX;

    result = Normalize(result);

    return result;
}


struct Vec3
{
    union
    {
        struct
        {
            real32 x, y, z;
        };
        real32 data[3];
    };  
};

inline real32 LengthSqr(Vec3 vec)
{
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

inline real32 Length(Vec3 vec)
{
    return sqrtf(LengthSqr(vec));
}

struct Vec4
{
    union
    {
        struct
        {
            real32 x, y, z, w;
        };
        real32 data[4];
    };
};

struct Rect
{
    union
    {
        struct
        {
            real32 x, y, width, height;            
        };

        struct
        {
            Vec2 min, max;
        };

        real32 data[4];
    };

};

struct Pointi
{
    union 
    {
        Vec2 p;
    };

    real32 data[2];
};

struct Line
{
    union
    {
        struct
        {
            Vec2 a, b;
        };

        Vec2 data[2]; 
    };
};

inline bool insideRect(Rect &rect, Pointi &p)
{
    union
    {
        real32 x, y;
    };

    union
    {
        Vec2 p;
    };

    real32 data[2];
}


inline bool rectIntersection(Rect &r1, Rect &r2) 
{
    bool isIntersects = !(r2.min.x > r1.max.x ||
                          r2.max.x < r1.min.x ||
                          r2.max.y < r1.min.y ||
                          r2.min.y > r1.max.y);
    return isIntersects;
}

struct Matrix4x4
{
    union 
    {
        struct
        {
            Vec4 row1;
            Vec4 row2;
            Vec4 row3;
            Vec4 row4;
        };

        real32 data[16];
    };
};

struct Quaternion
{
    union
    {
        real32 w, x, y, z;
    };

    real32 data[4];
};

struct Transform
{
    Vec3 pos;
    Quaternion quat;
    Vec2 scale;
};

inline Matrix4x4 createMat4x4()
{
    Matrix4x4 result = {};

    memset(result.data, 0, 16 * sizeof(real32));
    result.data[0 + 0 * 4] = 1.0f;
    result.data[1 + 1 * 4] = 1.0f;
    result.data[2 + 2 * 4] = 1.0f;
    result.data[3 + 3 * 4] = 1.0f;

    return result;
}

inline Vec3 createVec3(real32 x, real32 y, real32 z)
{
    Vec3 res = {};

    res.x = x;
    res.y = y;
    res.z = z;

    return res;
}

inline Rect createRect(real32 x, real32 y, real32 w, real32 h)
{
    Rect result = {};

    result.min.x = x;
    result.min.y = y;
    result.max.x = w;
    result.max.y = h;

    return result;
}

inline Rect createRect(Vec2 v1, Vec2 v2)
{
    Rect result = {};

    result.min = v1;
    result.max = v2;

    return result;
}

inline Quaternion createQuat()
{
    Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };
    return result;
}

inline Quaternion fromAxisAngle(Vec3 axis, Radian radians)
{
    Quaternion result = {};

    real32 precalc_sin = sin(radians * 0.5f);

    result.x = axis.x * precalc_sin;
    result.y = axis.y * precalc_sin;
    result.z = axis.z * precalc_sin;
    result.w = cos(radians * 0.5f);

    return result;
}

inline Quaternion fromAxisDegreeAngle(Vec3 axis, Degree degrees)
{
    return fromAxisAngle(axis, DegreesToRadians(degrees));
}

inline Quaternion operator*(Quaternion q1, Quaternion q2)
{
    Quaternion result = {};

    result.x = (q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y);
    result.y = (q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x);
    result.z = (q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w);
    result.w = (q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);

    return result;
}