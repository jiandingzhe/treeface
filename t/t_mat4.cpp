#include "TestFramework.h"
#include "treeface/math/mat4.h"

using namespace treeface;

template <typename T>
inline T DET3X3(T a, T b, T c, T d, T e, T f, T g, T h, T i)
{
    return (a*e*i + b*f*g + c*d*h -
            c*e*g - a*f*h - b*d*i);
}

float data[16] = {
    1,  2,  3,  4,
    5,  6,  7,  8,
    9,  10, 11, 12,
    13, 14, 15, 16
};

float data_rand[16] = {
    0.811147444955171,  0.0646941609188332, 0.559314987893355,  0.068969833288925,
    0.551752755779564,  0.921668696668334,  0.798661447034046,  0.175661833299351,
    0.527210205318998,  0.18036525329876,   0.893596997500747,  0.501034624865021,
    0.0616389883516497, 0.82792977251826,   0.0715971953036352, 0.307844576513268
};

void TestFramework::content()
{
    // 3D determinant
    {
        float det3 = det3x3<float>(simd_set<float, 16>(1, 2, 3, 0),
                                   simd_set<float, 16>(4, 5, 6, 0),
                                   simd_set<float, 16>(7, 8, 9, 0));
        float det3_ref = DET3X3<float>(1, 2, 3,
                                       4, 5, 6,
                                       7, 8, 9);
        IS_EPSILON(det3, det3_ref );
    }

    // object creation
    OK("create by columns");
    Mat4f mat(Vec4f(1,  2,  3,  4),
              Vec4f(5,  6,  7,  8),
              Vec4f(9,  10, 11, 12),
              Vec4f(13, 14, 15, 16));

    is(mat.get<0,0>(), 1,  "get 0,0");
    is(mat.get<1,0>(), 2,  "get 1,0");
    is(mat.get<2,0>(), 3,  "get 2,0");
    is(mat.get<3,0>(), 4,  "get 3,0");
    is(mat.get<0,1>(), 5,  "get 0,1");
    is(mat.get<1,1>(), 6,  "get 1,1");
    is(mat.get<2,1>(), 7,  "get 2,1");
    is(mat.get<3,1>(), 8,  "get 3,1");
    is(mat.get<0,2>(), 9,  "get 0,2");
    is(mat.get<1,2>(), 10, "get 1,2");
    is(mat.get<2,2>(), 11, "get 2,2");
    is(mat.get<3,2>(), 12, "get 3,2");
    is(mat.get<0,3>(), 13, "get 0,3");
    is(mat.get<1,3>(), 14, "get 1,3");
    is(mat.get<2,3>(), 15, "get 2,3");
    is(mat.get<3,3>(), 16, "get 3,3");

    {
        OK("create by array");
        Mat4f mat2(data);
        is(mat2.get<0,0>(), 1,  "get 0,0");
        is(mat2.get<1,0>(), 2,  "get 1,0");
        is(mat2.get<2,0>(), 3,  "get 2,0");
        is(mat2.get<3,0>(), 4,  "get 3,0");
        is(mat2.get<0,1>(), 5,  "get 0,1");
        is(mat2.get<1,1>(), 6,  "get 1,1");
        is(mat2.get<2,1>(), 7,  "get 2,1");
        is(mat2.get<3,1>(), 8,  "get 3,1");
        is(mat2.get<0,2>(), 9,  "get 0,2");
        is(mat2.get<1,2>(), 10, "get 1,2");
        is(mat2.get<2,2>(), 11, "get 2,2");
        is(mat2.get<3,2>(), 12, "get 3,2");
        is(mat2.get<0,3>(), 13, "get 0,3");
        is(mat2.get<1,3>(), 14, "get 1,3");
        is(mat2.get<2,3>(), 15, "get 2,3");
        is(mat2.get<3,3>(), 16, "get 3,3");
    }

    // transpose
    {
        Mat4f tr = mat;
        tr.transpose();
        is(tr.get<0,0>(), 1,  "get 0,0");
        is(tr.get<0,1>(), 2,  "get 0,1");
        is(tr.get<0,2>(), 3,  "get 0,2");
        is(tr.get<0,3>(), 4,  "get 0,3");
        is(tr.get<1,0>(), 5,  "get 1,0");
        is(tr.get<1,1>(), 6,  "get 1,1");
        is(tr.get<1,2>(), 7,  "get 1,2");
        is(tr.get<1,3>(), 8,  "get 1,3");
        is(tr.get<2,0>(), 9,  "get 2,0");
        is(tr.get<2,1>(), 10, "get 2,1");
        is(tr.get<2,2>(), 11, "get 2,2");
        is(tr.get<2,3>(), 12, "get 2,3");
        is(tr.get<3,0>(), 13, "get 3,0");
        is(tr.get<3,1>(), 14, "get 3,1");
        is(tr.get<3,2>(), 15, "get 3,2");
        is(tr.get<3,3>(), 16, "get 3,3");
    }

    // 4D determinant
    {
        OK("4D determinant");
        float det4 = mat.determinant();
        float det4_ref =
                1 * DET3X3(6,  7,  8,
                           10, 11, 12,
                           14, 15, 16)
                - 2 * DET3X3(5,  7,  8,
                             9,  11, 12,
                             13, 15, 16)
                + 3 * DET3X3(5,  6,  8,
                             9,  10, 12,
                             13, 14, 16)
                - 4 * DET3X3(5,  6,  7,
                             9,  10, 11,
                             13, 14, 15);
        IS_EPSILON(det4, det4_ref);
    }
    {
        Mat4f mat_rand(data_rand);
        float det4_rand = mat_rand.determinant();
        float det4_rand_ref =
                   0.811147444955171 * DET3X3(0.921668696668334,  0.798661447034046,  0.175661833299351,
                                              0.18036525329876,   0.893596997500747,  0.501034624865021,
                                              0.82792977251826,   0.0715971953036352, 0.307844576513268)
                -  0.551752755779564 * DET3X3(0.0646941609188332, 0.559314987893355,  0.068969833288925,
                                              0.18036525329876,   0.893596997500747,  0.501034624865021,
                                              0.82792977251826,   0.0715971953036352, 0.307844576513268)
                +  0.527210205318998 * DET3X3(0.0646941609188332, 0.559314987893355,  0.068969833288925,
                                              0.921668696668334,  0.798661447034046,  0.175661833299351,
                                              0.82792977251826,   0.0715971953036352, 0.307844576513268)
                - 0.0616389883516497 * DET3X3(0.0646941609188332, 0.559314987893355,  0.068969833288925,
                                              0.921668696668334,  0.798661447034046,  0.175661833299351,
                                              0.18036525329876,   0.893596997500747,  0.501034624865021);
        IS_EPSILON(det4_rand, det4_rand_ref);
    }

    {
        Mat4f trans(Vec4f(2, 2, 0, 0), Quatf(M_PI_2, Vec4f(0, 0, 1, 0)));
        OK("transform a zero point");
        Vec4f re = trans * Vec4f(0, 0, 0, 1);
        IS_EPSILON(re.get_x(), 2);
        IS_EPSILON(re.get_y(), 2);
        IS_EPSILON(re.get_z(), 0);
        IS_EPSILON(re.get_w(), 1);

        OK("transform a zero direction");
        re = trans * Vec4f(0, 0, 0, 0);
        IS_EPSILON(re.get_x(), 0);
        IS_EPSILON(re.get_y(), 0);
        IS_EPSILON(re.get_z(), 0);
        IS_EPSILON(re.get_w(), 0);

        OK("transform a direction");
        re = trans * Vec4f(2, 3, 0, 0);
        IS_EPSILON(re.get_x(), -3.0f);
        IS_EPSILON(re.get_y(), 2.0f);
        IS(re.get_z(), 0.0f);
        IS(re.get_w(), 0.0f);

        OK("transform a point");
        re = trans * Vec4f(2, 3, 0, 1);
        IS_EPSILON(re.get_x(), -1.0f);
        IS_EPSILON(re.get_y(), 4.0f);
        IS(re.get_z(), 0);
        IS(re.get_w(), 1);
    }

    {
        OK("matrix multiply");
        Mat4f mat_a(Vec4f(0.0732444811883433, 0.614589226522611, 0.190982955546257, 0.883557948606672),
                    Vec4f(0.218859207013413,  0.194015549714081, 0.444482891491443, 0.707075981464264),
                    Vec4f(0.639894840531642,  0.98940214404206,  0.294341245449498, 0.81921173701037),
                    Vec4f(0.395157428022674,  0.680578171124367, 0.908492824834408, 0.483502643037461));
        Mat4f mat_b(Vec4f(0.361240635001199,  0.471293407117145, 0.174365679306117, 0.126175795538934),
                    Vec4f(0.165263339539781,  0.498116686692796, 0.413253886761904, 0.0282075922414933),
                    Vec4f(0.0991675488097918, 0.923308927857533, 0.63599161921756,  0.588017167746763),
                    Vec4f(0.281098832406304,  0.305840146111009, 0.150343319707186, 0.979517544939988));
        Mat4f mat_combine = mat_b * mat_a;
        Vec4f input(1, 2, 3, 4);
        Vec4f out_discrete = mat_b * (mat_a * input);
        Vec4f out_combine = mat_combine * input;
        IS_EPSILON(out_discrete.get_x(), out_combine.get_x());
        IS_EPSILON(out_discrete.get_y(), out_combine.get_y());
        IS_EPSILON(out_discrete.get_z(), out_combine.get_z());
        IS_EPSILON(out_discrete.get_w(), out_combine.get_w());
    }

    {
        OK("matrix inverse");
        Mat4f fwd(data_rand);
        Mat4f inv(data_rand);
        float det = inv.inverse();

        Mat4f mul = fwd * inv;

        is_epsilon(simd_get_one<0, float>(mul.data[0]), 1.0f, "0,0 is 1");
        is_epsilon(simd_get_one<1, float>(mul.data[0]), 0.0f, "1,0 is 0");
        is_epsilon(simd_get_one<2, float>(mul.data[0]), 0.0f, "2,0 is 0");
        is_epsilon(simd_get_one<3, float>(mul.data[0]), 0.0f, "3,0 is 0");
        is_epsilon(simd_get_one<0, float>(mul.data[1]), 0.0f, "0,1 is 0");
        is_epsilon(simd_get_one<1, float>(mul.data[1]), 1.0f, "1,1 is 1");
        is_epsilon(simd_get_one<2, float>(mul.data[1]), 0.0f, "2,1 is 0");
        is_epsilon(simd_get_one<3, float>(mul.data[1]), 0.0f, "3,1 is 0");
        is_epsilon(simd_get_one<0, float>(mul.data[2]), 0.0f, "0,2 is 0");
        is_epsilon(simd_get_one<1, float>(mul.data[2]), 0.0f, "1,2 is 0");
        is_epsilon(simd_get_one<2, float>(mul.data[2]), 1.0f, "2,2 is 1");
        is_epsilon(simd_get_one<3, float>(mul.data[2]), 0.0f, "3,2 is 0");
        is_epsilon(simd_get_one<0, float>(mul.data[3]), 0.0f, "0,3 is 0");
        is_epsilon(simd_get_one<1, float>(mul.data[3]), 0.0f, "1,3 is 0");
        is_epsilon(simd_get_one<2, float>(mul.data[3]), 0.0f, "2,3 is 0");
        is_epsilon(simd_get_one<3, float>(mul.data[3]), 1.0f, "3,3 is 1");

    }
}
