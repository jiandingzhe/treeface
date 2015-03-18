#include "TestFramework.h"
#include "treeface/math/quat.h"

using namespace treeface;

void TestFramework::content()
{
    {
        OK("default constructor");
        Quatf a;
        IS(a.get_x(), 0);
        IS(a.get_y(), 0);
        IS(a.get_z(), 0);
        IS(a.get_w(), 1);
    }

    {
        OK("constructor with values");
        Quatf b(1, 2, 3, 4);
        IS(b.get_x(), 1);
        IS(b.get_y(), 2);
        IS(b.get_z(), 3);
        IS(b.get_w(), 4);

        OK("normalize");
        float len_before = b.normalize();
        IS_EPSILON(len_before, std::sqrt(30));
        IS_EPSILON(b.length(), 1.0);
        IS(b.get_x(), 1.0 / len_before);
        IS(b.get_y(), 2.0 / len_before);
        IS(b.get_z(), 3.0 / len_before);
        IS(b.get_w(), 4.0 / len_before);
    }

    {
        OK("constructor with angle and axis");
        Quatf c(M_PI/3, Vec4f(1, 2, 3, 4)); // the last component should be ommited
        IS_EPSILON(c.get_x(), 0.267261242 * sin(M_PI/6));
        IS_EPSILON(c.get_y(), 0.534522484 * sin(M_PI/6));
        IS_EPSILON(c.get_z(), 0.801783726 * sin(M_PI/6));
        IS_EPSILON(c.get_w(), cos(M_PI/6));

        IS_EPSILON(c.length(), 1);

        OK("get angle and axis");
        float angle;
        Vec4f axis;
        c.get_angle_axis(angle, axis);
        IS_EPSILON(angle, M_PI/3);
        IS_EPSILON(axis.get_x(), 0.267261242);
        IS_EPSILON(axis.get_y(), 0.534522484);
        IS_EPSILON(axis.get_z(), 0.801783726);
        IS_EPSILON(axis.get_w(), 0);
    }

    {
        OK("rotate vector from x to y");
        Quatf rot(M_PI_2, Vec4f(0, 0, 1, 0));
        Vec4f vec(2, 0, 0, 0);
        Vec4f re = rot.rotate(vec);
        IS_EPSILON(re.get_x(), 0.0);
        IS_EPSILON(re.get_y(), 2.0);
        IS_EPSILON(re.get_z(), 0.0);
        IS_EPSILON(re.get_w(), 0.0);
    }

    {
        OK("rotate vector from y to z");
        Quatf rot(M_PI_2, Vec4f(1, 0, 0, 0));
        Vec4f vec(0, 2, 0, 0);
        Vec4f re = rot.rotate(vec);
        IS_EPSILON(re.get_x(), 0.0);
        IS_EPSILON(re.get_y(), 0.0);
        IS_EPSILON(re.get_z(), 2.0);
        IS_EPSILON(re.get_w(), 0.0);
    }

    {
        OK("rotate vector from z to x");
        Quatf rot(M_PI_2, Vec4f(0, 1, 0, 0));
        Vec4f vec(0, 0, 2, 0);
        Vec4f re = rot.rotate(vec);
        IS_EPSILON(re.get_x(), 2.0);
        IS_EPSILON(re.get_y(), 0.0);
        IS_EPSILON(re.get_z(), 0.0);
        IS_EPSILON(re.get_w(), 0.0);
    }

    {
        OK("quaternion multiply");
        Quatf rot_x(M_PI_2, Vec4f(1, 0, 0, 0));
        Quatf rot_z(M_PI_2, Vec4f(0, 0, 1, 0));

        Quatf rot_combine = rot_x * rot_z;

        Vec4f vec(3, 0, 0, 0);
        Vec4f re = rot_combine.rotate(vec);

        IS_EPSILON(re.get_x(), 0.0);
        IS_EPSILON(re.get_y(), 0.0);
        IS_EPSILON(re.get_z(), 3.0);
        IS_EPSILON(re.get_w(), 0.0);
    }

}
