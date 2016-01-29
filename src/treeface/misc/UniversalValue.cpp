#include "treeface/misc/UniversalValue.h"

#include "treeface/misc/StringCast.h"
#include <treecore/MPL.h>

using namespace treecore;

static_assert( mpl_same_type<GLboolean, GLubyte>::value, "GLboolean is GLubyte" );

namespace treeface
{

bool UniversalValue::operator ==( const UniversalValue& peer ) const
{
    if (m_type != peer.m_type) return false;

    switch (m_type)
    {
    case TFGL_TYPE_BOOL:           return m_data.ui8 == peer.m_data.ui8;
    case TFGL_TYPE_BYTE:           return m_data.i8 == peer.m_data.i8;
    case TFGL_TYPE_SHORT:          return m_data.i16 == peer.m_data.i16;
    case TFGL_TYPE_INT:            return m_data.i32 == peer.m_data.i32;
    case TFGL_TYPE_UNSIGNED_BYTE:  return m_data.ui8 == peer.m_data.ui8;
    case TFGL_TYPE_UNSIGNED_SHORT: return m_data.ui16 == peer.m_data.ui16;
    case TFGL_TYPE_UNSIGNED_INT:   return m_data.ui32 == peer.m_data.ui32;
    case TFGL_TYPE_FLOAT:          return m_data.f32 == peer.m_data.f32;
    case TFGL_TYPE_DOUBLE:         return m_data.f64 == peer.m_data.f64;
    case TFGL_TYPE_VEC2F:          return m_data.vec2 == peer.m_data.vec2;
    case TFGL_TYPE_VEC3F:          return m_data.vec3 == peer.m_data.vec3;
    case TFGL_TYPE_VEC4F:          return m_data.vec4 == peer.m_data.vec4;
    //case TFGL_TYPE_MAT2F:          return m_data.mat2 == peer.m_data.mat2;
    //case TFGL_TYPE_MAT3F:          return m_data.mat3 == peer.m_data.mat3;
    //case TFGL_TYPE_MAT4F:          return m_data.mat4 == peer.m_data.mat4;
    default:
        die("unsupported data type in UniversalValue: %x", int(m_type));
    }
}

} // namespace treeface
