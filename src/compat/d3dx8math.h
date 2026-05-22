#pragma once

#include <cmath>
#include <cstring>

#include "d3d8types.h"

struct D3DXVECTOR2
{
    float x;
    float y;

    D3DXVECTOR2() = default;
    D3DXVECTOR2(float xIn, float yIn) : x(xIn), y(yIn) {}

    float &operator[](int index)
    {
        return (&x)[index];
    }

    const float &operator[](int index) const
    {
        return (&x)[index];
    }
};

struct D3DXVECTOR3
{
    float x;
    float y;
    float z;

    D3DXVECTOR3() = default;
    D3DXVECTOR3(float xIn, float yIn, float zIn) : x(xIn), y(yIn), z(zIn) {}

    float &operator[](int index)
    {
        return (&x)[index];
    }

    const float &operator[](int index) const
    {
        return (&x)[index];
    }
};

struct D3DXVECTOR4
{
    float x;
    float y;
    float z;
    float w;

    D3DXVECTOR4() = default;
    D3DXVECTOR4(float xIn, float yIn, float zIn, float wIn) : x(xIn), y(yIn), z(zIn), w(wIn) {}
};

struct D3DXQUATERNION
{
    float x;
    float y;
    float z;
    float w;
};

struct D3DXMATRIX
{
    float m[4][4];
};

inline D3DXVECTOR3 operator+(const D3DXVECTOR3 &lhs, const D3DXVECTOR3 &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

inline D3DXVECTOR3 operator-(const D3DXVECTOR3 &lhs, const D3DXVECTOR3 &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

inline D3DXVECTOR3 operator*(const D3DXVECTOR3 &lhs, float rhs)
{
    return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
}

inline D3DXVECTOR3 operator/(const D3DXVECTOR3 &lhs, float rhs)
{
    return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
}

inline D3DXVECTOR3 &operator+=(D3DXVECTOR3 &lhs, const D3DXVECTOR3 &rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

inline D3DXVECTOR3 &operator-=(D3DXVECTOR3 &lhs, const D3DXVECTOR3 &rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

inline D3DXVECTOR2 operator+(const D3DXVECTOR2 &lhs, const D3DXVECTOR2 &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline D3DXVECTOR2 operator-(const D3DXVECTOR2 &lhs, const D3DXVECTOR2 &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline D3DXVECTOR2 operator*(const D3DXVECTOR2 &lhs, float rhs)
{
    return {lhs.x * rhs, lhs.y * rhs};
}

inline D3DXVECTOR2 operator/(const D3DXVECTOR2 &lhs, float rhs)
{
    return {lhs.x / rhs, lhs.y / rhs};
}

inline D3DXMATRIX *D3DXMatrixIdentity(D3DXMATRIX *out)
{
    std::memset(out, 0, sizeof(*out));
    for (int i = 0; i < 4; ++i)
    {
        out->m[i][i] = 1.0f;
    }
    return out;
}

inline float D3DXVec3Length(const D3DXVECTOR3 *vec)
{
    return std::sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

inline D3DXVECTOR3 *D3DXVec3Normalize(D3DXVECTOR3 *out, const D3DXVECTOR3 *vec)
{
    const float length = D3DXVec3Length(vec);
    if (length == 0.0f)
    {
        out->x = 0.0f;
        out->y = 0.0f;
        out->z = 0.0f;
        return out;
    }
    out->x = vec->x / length;
    out->y = vec->y / length;
    out->z = vec->z / length;
    return out;
}

inline float D3DXToRadian(float degrees)
{
    return degrees * 3.14159265358979323846f / 180.0f;
}
