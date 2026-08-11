#include "renderer/Math.h"

#include <gtest/gtest.h>
#include <numbers>

using namespace porting_base;

struct Vec4 { float x, y, z, w; };

Vec4 Transform(const Mat4& m, float x, float y, float z, float w = 1.0f)
{
    return {
        m.m[0]*x + m.m[4]*y + m.m[8]*z  + m.m[12]*w,
        m.m[1]*x + m.m[5]*y + m.m[9]*z  + m.m[13]*w,
        m.m[2]*x + m.m[6]*y + m.m[10]*z + m.m[14]*w,
        m.m[3]*x + m.m[7]*y + m.m[11]*z + m.m[15]*w,
    };
}

TEST(Math, IdentityIsNeutral)
{
    Vec4 v = Transform(Mat4::Identity(), 1, 2, 3);
    EXPECT_NEAR(v.x, 1, 1e-5);
    EXPECT_NEAR(v.y, 2, 1e-5);
    EXPECT_NEAR(v.z, 3, 1e-5);
    EXPECT_NEAR(v.w, 1, 1e-5);
}

TEST(Math, TranslationMovesPoint)
{
    Vec4 v = Transform(Mat4::Translation(2, 3, 4), 1, 1, 1);
    EXPECT_NEAR(v.x, 3, 1e-5);
    EXPECT_NEAR(v.y, 4, 1e-5);
    EXPECT_NEAR(v.z, 5, 1e-5);
    EXPECT_NEAR(v.w, 1, 1e-5);
}

TEST(Math, RotationXDirection)
{
    const float halfPi = std::numbers::pi_v<float> / 2.0f;
    Vec4 v = Transform(Mat4::RotateX(halfPi), 0,1,0);
    EXPECT_NEAR(v.x, 0, 1e-5);
    EXPECT_NEAR(v.y, 0, 1e-5);
    EXPECT_NEAR(v.z, 1, 1e-5);
    EXPECT_NEAR(v.w, 1, 1e-5);
}

TEST(Math, RotationYDirection)
{
    const float halfPi = std::numbers::pi_v<float> / 2.0f;
    Vec4 v = Transform(Mat4::RotateY(halfPi), 1,0,0);
    EXPECT_NEAR(v.x, 0, 1e-5);
    EXPECT_NEAR(v.y, 0, 1e-5);
    EXPECT_NEAR(v.z, -1, 1e-5);
    EXPECT_NEAR(v.w, 1, 1e-5);
}

TEST(Math, CompositionOrder)
{
    const float halfPi = std::numbers::pi_v<float> / 2.0f;
    Vec4 v = Transform(Mat4::Translation(10, 0, 0) * Mat4::RotateY(halfPi), 1,0,0);
    EXPECT_NEAR(v.x, 10, 1e-5);
    EXPECT_NEAR(v.y, 0, 1e-5);
    EXPECT_NEAR(v.z, -1, 1e-5);
    EXPECT_NEAR(v.w, 1, 1e-5);   
}

TEST(Math, PerspectiveClipRange)
{
    const float zNear = 0.1f;
    const float zFar  = 100.0f;
    const Mat4 p = Mat4::Perspective(0.9f, 1.0f, zNear, zFar);

    const Vec4 nearPoint = Transform(p, 0.0f, 0.0f, -zNear);
    EXPECT_NEAR(nearPoint.z / nearPoint.w, -1.0f, 1e-4f);

    const Vec4 farPoint = Transform(p, 0.0f, 0.0f, -zFar);
    EXPECT_NEAR(farPoint.z / farPoint.w, 1.0f, 1e-4f);
}