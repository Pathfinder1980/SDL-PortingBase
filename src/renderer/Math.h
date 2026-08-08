#pragma once

#include <cmath>

namespace porting_base
{
    struct Mat4 
    { 
        float m[16] {};

        static Mat4 Identity()
        {
            return 
            {
                1.f, 0.f, 0.f, 0.f,
                0.f, 1.f, 0.f, 0.f,
                0.f, 0.f, 1.f, 0.f,
                0.f, 0.f, 0.f, 1.f
            };
        }

        static Mat4 Translation(float x, float y, float z)
        {
            Mat4 t = Identity();
            t.m[12] = x;
            t.m[13] = y;
            t.m[14] = z;
            return t;
        }

        static Mat4 RotateX(float radians)
        {
            Mat4 r = Identity();
            const float c = std::cos(radians);
            const float s = std::sin(radians);
            r.m[5]  = c;
            r.m[6]  = s;
            r.m[9]  = -s;
            r.m[10] = c;
            return r;
        }

        static Mat4 RotateY(float radians)
        {
            Mat4 r = Identity();
            const float c = std::cos(radians);
            const float s = std::sin(radians);
            r.m[0]  = c;
            r.m[2]  = -s;
            r.m[8]  = s;
            r.m[10] = c;
            return r;
        }
        
        static Mat4 Perspective(float fovY, float aspect, float zNear, float zFar)
        {
            Mat4 p = Identity();
            const float f = 1.0f / std::tan(fovY * 0.5f);            
            p.m[0] = f / aspect;
            p.m[5] = f;
            p.m[10] = (zFar + zNear)/ (zNear - zFar);
            p.m[11] = -1;
            p.m[14] = 2 * zFar * zNear / (zNear - zFar);
            p.m[15] = 0;
            return p;
        }

    };

    inline Mat4 operator*(const Mat4& a, const Mat4& b)
    {
        Mat4 r;
        for (int col = 0; col < 4; ++col)
        {
            for (int row = 0; row < 4; ++row)
            {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k)
                {
                    sum += a.m[k * 4 + row] * b.m[col * 4 + k];
                }
                r.m[col * 4 + row] = sum;
            }
        }
        return r;
    }

} // namespace porting_base

