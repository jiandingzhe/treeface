#ifndef TREEFACE_VEC3_H
#define TREEFACE_VEC3_H

#include <treecore/SimilarFloat.h>
#include <treecore/SimdObject.h>

namespace treeface
{

template<typename T>
struct Vec3
{
    typedef typename treecore::similar_float<T>::type FloatType;
    typedef treecore::SimdObject<T, 4> TmpType;
    typedef treecore::SimdObject<FloatType, 4> FloatTmpType;

    Vec3() noexcept
    {
    }

    Vec3(T x, T y, T z) noexcept: x(x), y(y), z(z)
    {
    }

    Vec3(const Vec3& peer) noexcept: x(peer.x), y(peer.y), z(peer.z)
    {
    }

    Vec3& operator = (const Vec3& peer) noexcept
    {
        x = peer.x;
        y = peer.y;
        z = peer.z;
        return *this;
    }

    void set(T x, T y, T z) noexcept
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3& operator += (const Vec3& peer) noexcept
    {
        TmpType a(x, y, z, 0);
        TmpType b(peer.x, peer.y, peer.z, 0);

        TmpType result = a + b;

        this->x = result.template get<0>();
        this->y = result.template get<1>();
        this->z = result.template get<2>();
        return *this;
    }

    Vec3& operator -= (const Vec3& peer) noexcept
    {
        TmpType a(x, y, z, 0);
        TmpType b(peer.x, peer.y, peer.z, 0);

        TmpType result = a - b;

        this->x = result.template get<0>();
        this->y = result.template get<1>();
        this->z = result.template get<2>();
        return *this;
    }

    Vec3& operator *= (T value) noexcept
    {
        TmpType a(x, y, z, 0);
        TmpType b(value, value, value, 0);

        TmpType result = a * b;

        this->x = result.template get<0>();
        this->y = result.template get<1>();
        this->z = result.template get<2>();
        return *this;
    }

    Vec3& operator /= (T value) noexcept
    {
        TmpType a(x, y, z, 0);
        TmpType b(value, value, value, 0);

        TmpType result = a / b;

        this->x = result.template get<0>();
        this->y = result.template get<1>();
        this->z = result.template get<2>();
        return *this;
    }

    Vec3 operator + (const Vec3& value) const noexcept
    {
        TmpType result = TmpType(x, y, z, 0) + TmpType(value.x, value.y, value.z, 0);
        return Vec3(result.template get<0>(), result.template get<1>(), result.template get<2>());
    }

    Vec3 operator - (const Vec3& value) const noexcept
    {
        TmpType result = TmpType(x, y, z, 0) - TmpType(value.x, value.y, value.z, 0);
        return Vec3(result.template get<0>(), result.template get<1>(), result.template get<2>());
    }

    Vec3 operator * (T value) const noexcept
    {
        TmpType result = TmpType(x, y, z, 0) * TmpType(value, value, value, 0);
        return Vec3(result.template get<0>(), result.template get<1>(), result.template get<2>());
    }

    Vec3 operator / (T value) const noexcept
    {
        TmpType result = TmpType(x, y, z, 0) / TmpType(value, value, value, 0);
        return Vec3(result.template get<0>(), result.template get<1>(), result.template get<2>());
    }

    FloatType operator * (const Vec3& value) const noexcept
    {
        FloatTmpType result = FloatTmpType(x, y, z, 0) * FloatTmpType(value.x, value.y, value.z, 0);
        return result.sum();
    }

    Vec3 operator ^ (const Vec3& peer) const noexcept
    {
        TmpType v1(x, y, z, 0);
        TmpType v2(peer.x, peer.y, peer.z, 0);
        TmpType v3(x, y, z, 0);
        TmpType v4(peer.x, peer.y, peer.z, 0);

        v1.template shuffle<1, 2, 0, 0>();
        v2.template shuffle<2, 0, 1, 0>();
        v3.template shuffle<2, 0, 1, 0>();
        v4.template shuffle<1, 2, 0, 0>();

        TmpType result = v1 * v2 - v3 * v4;
        return Vec3(result.template get<0>(),
                    result.template get<1>(),
                    result.template get<2>());
    }

    FloatType length2() const noexcept
    {
        FloatTmpType tmp(x, y, z, 0.0);
        FloatTmpType result = tmp * tmp;
        return result.sum();
    }

    FloatType length() const noexcept
    {
        return std::sqrt(length2());
    }

    FloatType normalize() noexcept
    {
        FloatTmpType tmp_values(x, y, z, 0.0f);
        FloatTmpType tmp_len2 = tmp_values * tmp_values;

        FloatType len = std::sqrt(tmp_len2.sum());
        FloatTmpType tmp_len(len, len, len, 0.0);

        tmp_values /= tmp_len;
        x = tmp_values.template get<0>();
        y = tmp_values.template get<1>();
        z = tmp_values.template get<2>();

        return len;
    }

    FloatType distance_from(const Vec3& start)
    {
        return (*this - start).length();
    }

    T x{};
    T y{};
    T z{};
};


template<typename T>
inline bool operator == (const Vec3<T>& a, const Vec3<T>& b) noexcept
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

template<typename T>
inline bool operator != (const Vec3<T>& a, const Vec3<T>& b) noexcept
{
    return a.x != b.x && a.y != b.y && a.z != b.z;
}

typedef Vec3<float> Vec3f;
typedef Vec3<treecore::int32> Vec3i;

} // namespace treeface

#endif // TREEFACE_VEC3_H
