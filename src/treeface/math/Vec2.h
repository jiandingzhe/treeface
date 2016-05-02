#ifndef TREECORE_VEC2_H
#define TREECORE_VEC2_H

#include <treecore/AlignedMalloc.h>
#include <treecore/ClassUtils.h>
#include <treecore/DebugUtils.h>
#include <treecore/IntTypes.h>
#include <treecore/FloatUtils.h>

#include <cmath>

namespace treeface
{

template<typename T>
struct Vec2
{
    typedef typename treecore::similar_float<T>::type FloatType;

    Vec2() = default;

    Vec2( T x, T y ) noexcept
        : x( x )
        , y( y )
    {}

    Vec2( const Vec2& peer ) = default;

    TREECORE_ALIGNED_ALLOCATOR( Vec2 );

    Vec2& operator = ( const Vec2& peer ) = default;

    void set( T x, T y ) noexcept
    {
        this->x = x;
        this->y = y;
    }

    Vec2 get_ortholog() const noexcept
    {
        return Vec2( -y, x );
    }

    Vec2 operator - () const noexcept
    {
        return Vec2( -x, -y );
    }

    bool is_zero() const noexcept
    {
        return x != T( 0 ) || y != T( 0 );
    }

    /**
     * @brief perform vector add
     */
    Vec2& operator += ( const Vec2& value ) noexcept
    {
        x += value.x;
        y += value.y;
        return *this;
    }

    /**
     * @brief perform vector subtract
     */
    Vec2& operator -= ( const Vec2& value ) noexcept
    {
        x -= value.x;
        y -= value.y;
        return *this;
    }

    /**
     * @breif multiply this vector with a single value
     */
    template<typename T2>
    Vec2& operator *= ( T2 value ) noexcept
    {
        x *= value;
        y *= value;
        return *this;
    }

    /**
     * @brief divide this vector with a single value
     */
    template<typename T2>
    Vec2& operator /= ( T2 value ) noexcept
    {
        x /= value;
        y /= value;
        return *this;
    }

    /**
     * @brief get length*length
     * @return result in float type whose size is similar with T
     */
    FloatType length2() const noexcept
    {
        return x * x + y * y;
    }

    /**
     * @brief get length
     * @return length value in float type whose size is similar with T
     */
    FloatType length() const noexcept
    {
        return std::sqrt( length2() );
    }

    FloatType distance_from( const Vec2& start )
    {
        FloatType dx( x - start.x );
        FloatType dy( y - start.y );
        return std::sqrt( dx * dx + dy * dy );
    }

    /**
     * @brief scale vector to make its length be one
     *
     * Note this is probably meaningless for integer type vectors.
     *
     * @return vector length
     */
    FloatType normalize() noexcept
    {
        FloatType len = (FloatType) length();
        if ( len == FloatType( 0 ) )
            return len;

        x /= len;
        y /= len;
        return len;
    }

    T x{};
    T y{};
};

/**
 * @brief vector add
 */
template<typename T>
Vec2<T> operator + ( const Vec2<T>& a, const Vec2<T>& b )
{
    Vec2<T> result( a );
    result += b;
    return result;
}

/**
 * @brief vector subtract
 */
template<typename T>
Vec2<T> operator - ( const Vec2<T>& a, const Vec2<T>& b )
{
    Vec2<T> result( a );
    result -= b;
    return result;
}

/**
 * @brief multiply vector with a single value
 */
template<typename T>
Vec2<T> operator * (T a, const Vec2<T>& b)
{
    Vec2<T> result( b );
    result *= a;
    return result;
}

template<typename T>
Vec2<T> operator * ( const Vec2<T>& a, T b )
{
    Vec2<T> result( a );
    result *= b;
    return result;
}

/**
 * @brief divide vector with a single value
 */
template<typename T>
Vec2<T> operator / ( const Vec2<T>& a, T b )
{
    Vec2<T> result( a );
    result /= b;
    return result;
}

/**
 * @brief dot multiply, or inner product
 *
 * Calculates ax*bx + ay*by
 *
 * @return result in float type whose size is similar with T
 */
template<typename T>
typename Vec2<T>::FloatType operator * ( const Vec2<T>& a, const Vec2<T>& b )
{
    return typename Vec2<T>::FloatType( a.x ) * typename Vec2<T>::FloatType( b.x ) + typename Vec2<T>::FloatType( a.y ) * typename Vec2<T>::FloatType( b.y );
}

/**
 * @brief cross multiply
 *
 * @return result in float type whose size is similar with T
 */
template<typename T>
typename Vec2<T>::FloatType operator % ( const Vec2<T>& a, const Vec2<T>& b )
{
    return typename Vec2<T>::FloatType( a.x ) * typename Vec2<T>::FloatType( b.y ) - typename Vec2<T>::FloatType( a.y ) * typename Vec2<T>::FloatType( b.x );
}

template<typename T>
bool operator == ( const Vec2<T>& a, const Vec2<T>& b )
{
    return a.x == b.x && a.y == b.y;
}

template<typename T>
bool operator != ( const Vec2<T>& a, const Vec2<T>& b )
{
    return a.x != b.x || a.y != b.y;
}

typedef Vec2<float>           Vec2f;
typedef Vec2<treecore::int32> Vec2i;

} // namespace treeface

#endif // TREECORE_VEC2_H
