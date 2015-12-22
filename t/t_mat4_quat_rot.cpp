#include "TestFramework.h"
#include "treeface/math/mat4.h"

using namespace treeface;

void TestFramework::content()
{
    {
        Quatf rot_quat(3.1415927f/6, Vec3f(0.0f, 0.0f, 1.0f));
        Mat4f rot_mat;
        rot_mat.set_rotate(rot_quat);

        {
            float r0_c0 = rot_mat.get<0, 0>();
            float r0_c1 = rot_mat.get<0, 1>();
            float r0_c2 = rot_mat.get<0, 2>();
            float r0_c3 = rot_mat.get<0, 3>();
            float r1_c0 = rot_mat.get<1, 0>();
            float r1_c1 = rot_mat.get<1, 1>();
            float r1_c2 = rot_mat.get<1, 2>();
            float r1_c3 = rot_mat.get<1, 3>();
            float r2_c0 = rot_mat.get<2, 0>();
            float r2_c1 = rot_mat.get<2, 1>();
            float r2_c2 = rot_mat.get<2, 2>();
            float r2_c3 = rot_mat.get<2, 3>();
            float r3_c0 = rot_mat.get<3, 0>();
            float r3_c1 = rot_mat.get<3, 1>();
            float r3_c2 = rot_mat.get<3, 2>();
            float r3_c3 = rot_mat.get<3, 3>();
            IS_EPSILON(r0_c0, 1.732051f/2); IS_EPSILON(r0_c1, -0.5f);       LE(std::abs(r0_c2), 0.00001f);  LE(std::abs(r0_c3), 0.00001f);
            IS_EPSILON(r1_c0, 0.5f);        IS_EPSILON(r1_c1, 1.732051f/2); LE(std::abs(r1_c2), 0.00001f);  LE(std::abs(r1_c3), 0.00001f);
            LE(std::abs(r2_c0), 0.00001f); LE(std::abs(r2_c1), 0.00001f);   IS_EPSILON(r2_c2, 1.0f);        LE(std::abs(r2_c3), 0.00001f);
            LE(std::abs(r3_c0), 0.00001f); LE(std::abs(r3_c1), 0.00001f);   LE(std::abs(r3_c2), 0.00001f);  IS_EPSILON(r3_c3, 1.0f);
        }

        {
            Vec4f unit_x_rot = rot_mat * Vec4f(1.0f, 0.0f, 0.0f, 0.0f);
            IS_EPSILON(unit_x_rot.get_x(), 1.732051f/2);
            IS_EPSILON(unit_x_rot.get_y(), 0.5f);
            LE(std::abs(unit_x_rot.get_z()), 0.00001f);
            LE(std::abs(unit_x_rot.get_w()), 0.00001f);

            Vec4f unit_x_rot_rot = rot_mat * unit_x_rot;
            IS_EPSILON(unit_x_rot_rot.get_x(), 0.5f);
            IS_EPSILON(unit_x_rot_rot.get_y(), 1.732051f/2);
            LE(std::abs(unit_x_rot_rot.get_z()), 0.00001f);
            LE(std::abs(unit_x_rot_rot.get_w()), 0.00001f);
        }

        {
            Vec4f neg_y_rot = rot_mat * Vec4f(0.0f, -1.0f, 0.0f, 0.0f);
            IS_EPSILON(neg_y_rot.get_x(), 0.5f);
            IS_EPSILON(neg_y_rot.get_y(), -1.732051f/2);
            LE(std::abs(neg_y_rot.get_z()), 0.00001f);
            LE(std::abs(neg_y_rot.get_w()), 0.00001f);

            Vec4f neg_y_rot_rot = rot_mat * neg_y_rot;
            IS_EPSILON(neg_y_rot_rot.get_x(), 1.732051f/2);
            IS_EPSILON(neg_y_rot_rot.get_y(), -0.5f);
            LE(std::abs(neg_y_rot_rot.get_z()), 0.00001f);
            LE(std::abs(neg_y_rot_rot.get_w()), 0.00001f);
        }
    }

    {
        Quatf rot_quat(3.1415927f/5, Vec3f(1.0f, 2.0f, 3.0f));
        Mat4f rot_mat;
        rot_mat.set_rotate(rot_quat);

        printf("quat:\n"
               "%f\t%f\t%f\t%f\n",
               rot_quat.get_x(), rot_quat.get_y(), rot_quat.get_z(), rot_quat.get_w());

        printf("mat:\n"
               "%f\t%f\t%f\t%f\n"
               "%f\t%f\t%f\t%f\n"
               "%f\t%f\t%f\t%f\n"
               "%f\t%f\t%f\t%f\n",
               rot_mat.get<0, 0>(), rot_mat.get<0, 1>(), rot_mat.get<0, 2>(), rot_mat.get<0, 3>(),
               rot_mat.get<1, 0>(), rot_mat.get<1, 1>(), rot_mat.get<1, 2>(), rot_mat.get<1, 3>(),
               rot_mat.get<2, 0>(), rot_mat.get<2, 1>(), rot_mat.get<2, 2>(), rot_mat.get<2, 3>(),
               rot_mat.get<3, 0>(), rot_mat.get<3, 1>(), rot_mat.get<3, 2>(), rot_mat.get<3, 3>()
               );
    }

    {
        Quatf rot_quat(3.1415927f/2, Vec3f(1.0f, 1.0f, 0.0f));
        Mat4f rot_mat;
        rot_mat.set_rotate(rot_quat);

        printf("quat:\n"
               "%f\t%f\t%f\t%f\n",
               rot_quat.get_x(), rot_quat.get_y(), rot_quat.get_z(), rot_quat.get_w());

        printf("mat:\n"
               "%f\t%f\t%f\t%f\n"
               "%f\t%f\t%f\t%f\n"
               "%f\t%f\t%f\t%f\n"
               "%f\t%f\t%f\t%f\n",
               rot_mat.get<0, 0>(), rot_mat.get<0, 1>(), rot_mat.get<0, 2>(), rot_mat.get<0, 3>(),
               rot_mat.get<1, 0>(), rot_mat.get<1, 1>(), rot_mat.get<1, 2>(), rot_mat.get<1, 3>(),
               rot_mat.get<2, 0>(), rot_mat.get<2, 1>(), rot_mat.get<2, 2>(), rot_mat.get<2, 3>(),
               rot_mat.get<3, 0>(), rot_mat.get<3, 1>(), rot_mat.get<3, 2>(), rot_mat.get<3, 3>()
               );

        {
            Vec4f unit_z_rot = rot_mat * Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
            IS_EPSILON(unit_z_rot.get_x(), 1.414214f/2);
            IS_EPSILON(unit_z_rot.get_y(), -1.414214f/2);
            LE(std::abs(unit_z_rot.get_z()), 0.00001f);
            LE(std::abs(unit_z_rot.get_w()), 0.00001f);

            Vec4f unit_z_rot_rot = rot_mat * unit_z_rot;
            LE(std::abs(unit_z_rot_rot.get_x()), 0.00001f);
            LE(std::abs(unit_z_rot_rot.get_y()), 0.00001f);
            IS_EPSILON(unit_z_rot_rot.get_z(), -1.0f);
            LE(std::abs(unit_z_rot_rot.get_w()), 0.00001f);
        }
    }
}
