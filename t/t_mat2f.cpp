#include "TestFramework.h"

#include "treeface/math/Mat2.h"

void TestFramework::content()
{
    {
        treeface::Mat2f mat_fwd(123.456f, 789.123f, 654.321f, 234.578f);
        treeface::Mat2f mat_inv = mat_fwd;
        float det = mat_inv.inverse();
        IS_EPSILON(det, -487379.7);
        IS_EPSILON(mat_inv.data.get<0>(), -0.0004813044f);
        IS_EPSILON(mat_inv.data.get<1>(),  0.001619113f);
        IS_EPSILON(mat_inv.data.get<2>(),  0.001342528f);
        IS_EPSILON(mat_inv.data.get<3>(), -0.0002533056f);

        {
            treeface::Mat2f mat_fwd_inv = mat_fwd * mat_inv;
            IS_EPSILON(mat_fwd_inv.data.get<0>(), 1.0f);
            LE(std::abs(mat_fwd_inv.data.get<1>()), 0.00001f);
            LE(std::abs(mat_fwd_inv.data.get<2>()), 0.00001f);
            IS_EPSILON(mat_fwd_inv.data.get<3>(), 1.0f);
        }

        {
            treeface::Mat2f mat_inv_fwd = mat_inv * mat_fwd;
            IS_EPSILON(mat_inv_fwd.data.get<0>(), 1.0f);
            LE(std::abs(mat_inv_fwd.data.get<1>()), 0.00001f);
            LE(std::abs(mat_inv_fwd.data.get<2>()), 0.00001f);
            IS_EPSILON(mat_inv_fwd.data.get<3>(), 1.0f);
        }

        {
            treeface::Vec2f vec_fwd_inv = mat_inv * (mat_fwd * treeface::Vec2f(444.333f, 222.111f));
            IS_EPSILON(vec_fwd_inv.x, 444.333f);
            IS_EPSILON(vec_fwd_inv.y, 222.111f);
        }

        {
            treeface::Vec2f vec_inv_fwd = mat_fwd * (mat_inv * treeface::Vec2f(444.333f, 222.111f));
            IS_EPSILON(vec_inv_fwd.x, 444.333f);
            IS_EPSILON(vec_inv_fwd.y, 222.111f);
        }
    }

    {
        treeface::Mat2f mat_rot;
        mat_rot.set_rotate(3.1415927f/6);

        {
            treeface::Vec2f rot_unit_x = mat_rot * treeface::Vec2f(1.0f, 0.0f);
            IS_EPSILON(rot_unit_x.x, 1.732051f/2);
            IS_EPSILON(rot_unit_x.y, 0.5f);
        }

        {
            treeface::Vec2f rot_rot_unit_x = mat_rot * (mat_rot * treeface::Vec2f(1.0f, 0.0f));
            IS_EPSILON(rot_rot_unit_x.x, 0.5f);
            IS_EPSILON(rot_rot_unit_x.y, 1.732051f/2);
        }

        {
            treeface::Vec2f rot_result = mat_rot * treeface::Vec2f(1.0f, 1.732051f);
            LE(std::abs(rot_result.x), 0.00001f);
            IS_EPSILON(rot_result.y, 2.0f);
        }

        mat_rot.set_rotate(-3.1415927f/3);
        {
            {
                treeface::Vec2f rot_unit_y = mat_rot * treeface::Vec2f(0.0f, 2.0f);
                IS_EPSILON(rot_unit_y.x, 1.732051f);
                IS_EPSILON(rot_unit_y.y, 1.0f);
            }
        }
    }
}

