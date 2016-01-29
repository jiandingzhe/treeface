#include "treeface/math/Mat3.h"
#include "TestFramework.h"

using namespace treecore;
using namespace treeface;

float data_rand[9] = {
    0.8111474f,  0.0646942f, 0.5593150f,
    0.5517528f,  0.9216687f, 0.7986614f,
    0.5272102f,  0.1803653f, 0.8935970f,
};

float data_rand_inv[9] = {
    2.0564934f, 0.1303424f, -1.4036832f,
    -0.2178369f, 1.3011776f, -1.0265935f,
    -1.1693348f, -0.3395324f, 2.1544364f
};

void TestFramework::content()
{
    {
        float det3 = treeface::det3x3(treecore::SimdObject<float, 4>(2.0f, 0.0f, 0.0f, 0.0f),
                                      treecore::SimdObject<float, 4>(0.0f, 2.0f, 0.0f, 0.0f),
                                      treecore::SimdObject<float, 4>(0.0f, 0.0f, 2.0f, 0.0f));
        IS(det3, 8.0f);
    }
    {
        float det3 = treeface::det3x3(treecore::SimdObject<float, 4>(2.0f, 0.0f, 0.0f, 0.0f),
                                      treecore::SimdObject<float, 4>(0.0f, 3.0f, 0.0f, 0.0f),
                                      treecore::SimdObject<float, 4>(0.0f, 0.0f, 5.0f, 0.0f));
        IS(det3, 30.0f);
    }

    OK("default constructor creates unit matrix");
    {
        Mat3f mat;
        IS(mat.data[0], 1.0f);
        IS(mat.data[1], 0.0f);
        IS(mat.data[2], 0.0f);

        IS(mat.data[3], 0.0f);
        IS(mat.data[4], 1.0f);
        IS(mat.data[5], 0.0f);

        IS(mat.data[6], 0.0f);
        IS(mat.data[7], 0.0f);
        IS(mat.data[8], 1.0f);
    }

    OK("multiply two scale-only matrix");
    {
        Mat3f mul_result =
                Mat3f(2.0f, 0.0f, 0.0f,
                      0.0f, 3.0f, 0.0f,
                      0.0f, 0.0f, 5.0f)
                *
                Mat3f(7.0f, 0.0f, 0.0f,
                      0.0f, 11.0f, 0.0f,
                      0.0f, 0.0f, 13.0f);

        IS_EPSILON(mul_result.data[0], 14.0f);
        LT(std::abs(mul_result.data[1]), 1.0f/10000);
        LT(std::abs(mul_result.data[2]), 1.0f/10000);

        LT(std::abs(mul_result.data[3]), 1.0f/10000);
        IS_EPSILON(mul_result.data[4], 33.0f);
        LT(std::abs(mul_result.data[5]), 1.0f/10000);

        LT(std::abs(mul_result.data[6]), 1.0f/10000);
        LT(std::abs(mul_result.data[7]), 1.0f/10000);
        IS_EPSILON(mul_result.data[8], 65.0f);
    }

    OK("multiply unit matix and another matrix");
    {
        Mat3f mul_result = Mat3f() * Mat3f(data_rand);
        IS(mul_result.data[0], data_rand[0]);
        IS(mul_result.data[1], data_rand[1]);
        IS(mul_result.data[2], data_rand[2]);
        IS(mul_result.data[3], data_rand[3]);
        IS(mul_result.data[4], data_rand[4]);
        IS(mul_result.data[5], data_rand[5]);
        IS(mul_result.data[6], data_rand[6]);
        IS(mul_result.data[7], data_rand[7]);
        IS(mul_result.data[8], data_rand[8]);
    }

    OK("multiply of two counteract scale matrix");
    {
        Mat3f fwd(2.0f, 0.0f, 0.0f,
                  0.0f, 3.0f, 0.0f,
                  0.0f, 0.0f, 5.0f);
        Mat3f inv(1.0f/2, 0.0f,   0.0f,
                  0.0f,   1.0f/3, 0.0f,
                  0.0f,   0.0f,   1.0f/5);

        Mat3f mul_result = fwd * inv;

        IS(mul_result.data[0], 1.0f);
        IS(mul_result.data[1], 0.0f);
        IS(mul_result.data[2], 0.0f);
        IS(mul_result.data[3], 0.0f);
        IS(mul_result.data[4], 1.0f);
        IS(mul_result.data[5], 0.0f);
        IS(mul_result.data[6], 0.0f);
        IS(mul_result.data[7], 0.0f);
        IS(mul_result.data[8], 1.0f);
    }

    OK("inversion of unit matrix");
    {
        Mat3f inv;
        inv.inverse();

        IS(inv.data[0], 1.0f);
        IS(inv.data[1], 0.0f);
        IS(inv.data[2], 0.0f);

        IS(inv.data[3], 0.0f);
        IS(inv.data[4], 1.0f);
        IS(inv.data[5], 0.0f);

        IS(inv.data[6], 0.0f);
        IS(inv.data[7], 0.0f);
        IS(inv.data[8], 1.0f);
    }

    OK("inversion of scale matrix");
    {
        Mat3f fwd(2.0f, 0.0f, 0.0f,
                  0.0f, 3.0f, 0.0f,
                  0.0f, 0.0f, 5.0f);
        Mat3f inv = fwd;
        inv.inverse();

        IS_EPSILON(inv.data[0], 1.0f/2);
        LT(std::abs(inv.data[1]), 1.0f/10000);
        LT(std::abs(inv.data[2]), 1.0f/10000);

        LT(std::abs(inv.data[3]), 1.0f/10000);
        IS_EPSILON(inv.data[4], 1.0f/3);
        LT(std::abs(inv.data[5]), 1.0f/10000);

        LT(std::abs(inv.data[6]), 1.0f/10000);
        LT(std::abs(inv.data[7]), 1.0f/10000);
        IS_EPSILON(inv.data[8], 1.0f/5);
    }

    OK("inversion of random matrix");
    {
        Mat3f inv(data_rand);
        float det = inv.inverse();

        IS_EPSILON(det, 0.3304409f);

        IS_EPSILON(inv.data[0], data_rand_inv[0]);
        IS_EPSILON(inv.data[1], data_rand_inv[1]);
        IS_EPSILON(inv.data[2], data_rand_inv[2]);
        IS_EPSILON(inv.data[3], data_rand_inv[3]);
        IS_EPSILON(inv.data[4], data_rand_inv[4]);
        IS_EPSILON(inv.data[5], data_rand_inv[5]);
        IS_EPSILON(inv.data[6], data_rand_inv[6]);
        IS_EPSILON(inv.data[7], data_rand_inv[7]);
        IS_EPSILON(inv.data[8], data_rand_inv[8]);
    }

    OK("multiply of inverted matrix and original matrix");
    {
        Mat3f fwd(data_rand);
        Mat3f inv(data_rand);
        inv.inverse();

        Mat3f inv_x_fwd = inv * fwd;

        IS_EPSILON(inv_x_fwd.data[0], 1.0f);
        LT(std::abs(inv_x_fwd.data[1]), 1.0f/10000);
        LT(std::abs(inv_x_fwd.data[2]), 1.0f/10000);

        LT(std::abs(inv_x_fwd.data[3]), 1.0f/10000);
        IS_EPSILON(inv_x_fwd.data[4], 1.0f);
        LT(std::abs(inv_x_fwd.data[5]), 1.0f/10000);

        LT(std::abs(inv_x_fwd.data[6]), 1.0f/10000);
        LT(std::abs(inv_x_fwd.data[7]), 1.0f/10000);
        IS_EPSILON(inv_x_fwd.data[8], 1.0f);
    }

    OK("matrix x vector, scale only");
    {
        Vec3f scaled_vec = Mat3f(7.0f, 0.0f, 0.0f,
                                 0.0f, 11.0f, 0.0f,
                                 0.0f, 0.0f, 13.0f) * Vec3f(2.0f, 3.0f, 5.0f);

        IS(scaled_vec.x, 14.0f);
        IS(scaled_vec.y, 33.0f);
        IS(scaled_vec.z, 65.0f);
    }

    OK("matrix x vector");
    {
        Vec3f mul_vec = Mat3f(data_rand) * Vec3f(2.0f, 3.0f, 5.0f);

        IS_EPSILON(mul_vec.x, 5.913604f);
        IS_EPSILON(mul_vec.y, 3.796221f);
        IS_EPSILON(mul_vec.z, 7.982599f);
    }
}
