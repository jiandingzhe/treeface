#include "treeface/gl/error.h"
#include "treeface/gl/imageref.h"
#include "treeface/gl/texture.h"
#include "treeface/image.h"
#include "treeface/imagemanager.h"
#include "treeface/misc/error.h"
#include "treeface/misc/propertyvalidator.h"
#include "treeface/misc/stringcast.h"

#include <treecore/DynamicObject.h>
#include <treecore/Logger.h>
#include <treecore/HashSet.h>
#include <treecore/NamedValueSet.h>
#include <treecore/String.h>
#include <treecore/StringRef.h>
#include <treecore/Variant.h>

using namespace treecore;

namespace treeface {

const GLenum TEXTURE_UNITS[32] = {
    GL_TEXTURE0,
    GL_TEXTURE1,
    GL_TEXTURE2,
    GL_TEXTURE3,
    GL_TEXTURE4,
    GL_TEXTURE5,
    GL_TEXTURE6,
    GL_TEXTURE7,
    GL_TEXTURE8,
    GL_TEXTURE9,
    GL_TEXTURE10,
    GL_TEXTURE11,
    GL_TEXTURE12,
    GL_TEXTURE13,
    GL_TEXTURE14,
    GL_TEXTURE15,
    GL_TEXTURE16,
    GL_TEXTURE17,
    GL_TEXTURE18,
    GL_TEXTURE19,
    GL_TEXTURE20,
    GL_TEXTURE21,
    GL_TEXTURE22,
    GL_TEXTURE23,
    GL_TEXTURE24,
    GL_TEXTURE25,
    GL_TEXTURE26,
    GL_TEXTURE27,
    GL_TEXTURE28,
    GL_TEXTURE29,
    GL_TEXTURE30,
    GL_TEXTURE31,
};

inline GLuint _gen_texture_()
{
    GLuint result = 0;
    glGenTextures( 1, &result );
    if (result == 0) die( "failed to generate texture" );
    return result;
}

inline void _gl_tex_image_( GLenum target, GLint level, TextureCompatibleImageRef img )
{
    glTexImage2D( target, level, img.internal_format, img.width, img.height, 0, img.format, img.type, img.data );
}

inline void _gl_tex_image_( GLenum target, GLint level, TextureCompatibleImageArrayRef img )
{
    glTexImage3D( target, level, img.internal_format, img.width, img.height, img.num_frame, 0, img.format, img.type, img.data );
}

inline void _gl_tex_image_( GLenum target, GLint level, TextureCompatibleVoxelBlockRef img )
{
    glTexImage3D( target, level, img.internal_format, img.width, img.height, img.depth, 0, img.format, img.type, img.data );
}

inline void _check_error_unbind_( GLTextureType type, const String& msg )
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        glBindTexture( type, 0 );
        switch (err)
        {
        case GL_INVALID_ENUM:      throw GLInvalidEnum( "invalid enum while " + msg );
        case GL_INVALID_VALUE:     throw GLInvalidValue( "invalid value while " + msg );
        case GL_INVALID_OPERATION: throw GLInvalidOperation( "invalid operation while " + msg );
        default:
            throw std::runtime_error( "" );
        }
    }
}

Texture::Texture(  TextureCompatibleImageRef image, treecore::uint32 num_gen_mipmap  )
    : m_type( TFGL_TEXTURE_2D )
    , m_texture( _gen_texture_() )
{
    glBindTexture( m_type, m_texture );

    glTexParameteri( m_type, GL_TEXTURE_BASE_LEVEL, 0 );
    glTexParameteri( m_type, GL_TEXTURE_MAX_LEVEL,  num_gen_mipmap );

    _gl_tex_image_( m_type, 0, image );
    _check_error_unbind_( m_type, "assigning 2D texture image" );

    if (num_gen_mipmap)
    {
        glGenerateMipmap( m_type );
        _check_error_unbind_( m_type, "generating 2D texture mipmaps" );
    }

    glBindTexture( m_type, 0 );
}

Texture::Texture( GLsizei width, GLsizei height, GLsizei levels, GLInternalImageFormat internal_fmt )
    : m_texture( _gen_texture_() )
    , m_type( TFGL_TEXTURE_2D )
    , m_immutable( true )
{
    glBindTexture( m_type, m_texture );

    glTexStorage2D( m_type, levels, internal_fmt, width, height );
    _check_error_unbind_( m_type, "creating immutable 2D texture" );

    glBindTexture( m_type, 0 );
}

Texture::Texture( treecore::ArrayRef<TextureCompatibleImageRef> images )
    : m_texture( _gen_texture_() )
    , m_type( TFGL_TEXTURE_2D )
{
    glBindTexture( m_type, m_texture );

    glTexParameteri( m_type, GL_TEXTURE_BASE_LEVEL, 0 );
    glTexParameteri( m_type, GL_TEXTURE_MAX_LEVEL,  images.size() - 1 );

    for (int level = 0; level < images.size(); level++)
    {
        _gl_tex_image_( m_type, level, images[level] );
        _check_error_unbind_( m_type, "assigning 2D texture data for mipmap level " + String( level ) );
    }

    glBindTexture( m_type, 0 );
}

Texture::Texture( TextureCompatibleImageArrayRef images, uint32 num_gen_mipmap )
    : m_texture( _gen_texture_() )
    , m_type( TFGL_TEXTURE_2D_ARRAY )
{
    glBindTexture( m_type, m_texture );

    glTexParameteri( m_type, GL_TEXTURE_BASE_LEVEL, 0 );
    glTexParameteri( m_type, GL_TEXTURE_MAX_LEVEL,  num_gen_mipmap );

    _gl_tex_image_( m_type, 0, images );
    _check_error_unbind_( m_type, "assigning 2D texture array image data" );

    if (num_gen_mipmap > 0)
    {
        glGenerateMipmap( m_type );
        _check_error_unbind_( m_type, "generating 2D texture array mipmaps" );
    }

    glBindTexture( m_type, 0 );
}

Texture::Texture( TextureCompatibleImageRef img_x_plus, TextureCompatibleImageRef img_x_minus,
                  TextureCompatibleImageRef img_y_plus, TextureCompatibleImageRef img_y_minus,
                  TextureCompatibleImageRef img_z_plus, TextureCompatibleImageRef img_z_minus,
                  treecore::uint32 num_gen_mipmap )
    : m_texture( _gen_texture_() )
    , m_type( TFGL_TEXTURE_CUBE )
{
    glBindTexture( m_type, m_texture );

    glTexParameteri( m_type, GL_TEXTURE_BASE_LEVEL, 0 );
    glTexParameteri( m_type, GL_TEXTURE_MAX_LEVEL,  num_gen_mipmap );

    _gl_tex_image_( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, img_x_plus );
    _check_error_unbind_( m_type, "assigning 2D cube texture x+ image data" );

    _gl_tex_image_( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, img_x_minus );
    _check_error_unbind_( m_type, "assigning 2D cube texture x- image data" );

    _gl_tex_image_( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, img_y_plus );
    _check_error_unbind_( m_type, "assigning 2D cube texture y+ image data" );

    _gl_tex_image_( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, img_y_minus );
    _check_error_unbind_( m_type, "assigning 2D cube texture y- image data" );

    _gl_tex_image_( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, img_z_plus );
    _check_error_unbind_( m_type, "assigning 2D cube texture z+ image data" );

    _gl_tex_image_( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, img_z_minus );
    _check_error_unbind_( m_type, "assigning 2D cube texture z- image data" );

    if (num_gen_mipmap > 0)
    {
        glGenerateMipmap( m_type );
        _check_error_unbind_( m_type, "generating 2D texture array mipmaps" );
    }

    glBindTexture( m_type, 0 );
}

Texture::Texture( TextureCompatibleVoxelBlockRef voxel, uint32 num_gen_mipmap )
    : m_texture( _gen_texture_() )
    , m_type( TFGL_TEXTURE_3D )
{
    glBindTexture( m_type, m_texture );

    glTexParameteri( m_type, GL_TEXTURE_BASE_LEVEL, 0 );
    glTexParameteri( m_type, GL_TEXTURE_MAX_LEVEL,  num_gen_mipmap );

    _gl_tex_image_( m_type, 0, voxel );
    _check_error_unbind_( m_type, "assigning 3D texture image data" );

    if (num_gen_mipmap > 0)
    {
        glGenerateMipmap( m_type );
        _check_error_unbind_( m_type, "generating 3D texture mipmaps" );
    }

    glBindTexture( m_type, 0 );
}

#define KEY_NAME          "name"
#define KEY_IMG           "image"
#define KEY_MAG_LINEAR    "mag_filter_linear"
#define KEY_MIN_LINEAR    "min_filter_linear"
#define KEY_MIPMAP        "mipmap"
#define KEY_MIPMAP_LINEAR "mipmap_filter_linear"
#define KEY_SLICES        "slices"
#define KEY_TYPE          "type"
#define KEY_WRAP_S        "wrap_s"
#define KEY_WRAP_T        "wrap_t"
#define KEY_POL_SOLO      "solo_policy"
#define KEY_POL_DUAL      "dual_policy"
#define KEY_POL_INT       "int_policy"

Result _validate_keys_( NamedValueSet& kv )
{
    static PropertyValidator* validator = nullptr;
    if (!validator)
    {
        validator = new PropertyValidator();
        validator->add_item( KEY_NAME,          PropertyValidator::ITEM_SCALAR,                                 true );
        validator->add_item( KEY_IMG,           PropertyValidator::ITEM_SCALAR | PropertyValidator::ITEM_ARRAY, true );
        validator->add_item( KEY_MAG_LINEAR,    PropertyValidator::ITEM_SCALAR,                                 false );
        validator->add_item( KEY_MIN_LINEAR,    PropertyValidator::ITEM_SCALAR,                                 false );
        validator->add_item( KEY_MIPMAP,        PropertyValidator::ITEM_SCALAR,                                 false );
        validator->add_item( KEY_MIPMAP_LINEAR, PropertyValidator::ITEM_SCALAR,                                 false );
        validator->add_item( KEY_SLICES,        PropertyValidator::ITEM_SCALAR,                                 false );
        validator->add_item( KEY_TYPE,          PropertyValidator::ITEM_SCALAR,                                 true );
        validator->add_item( KEY_WRAP_S,        PropertyValidator::ITEM_SCALAR,                                 false );
        validator->add_item( KEY_WRAP_T,        PropertyValidator::ITEM_SCALAR,                                 false );
        validator->add_item( KEY_POL_SOLO,      PropertyValidator::ITEM_SCALAR,                                 false );
        validator->add_item( KEY_POL_DUAL,      PropertyValidator::ITEM_SCALAR,                                 false );
        validator->add_item( KEY_POL_INT,       PropertyValidator::ITEM_SCALAR,                                 false );
    }
    return validator->validate( kv );
}

Texture::Texture( const treecore::var& tex_node )
    : m_texture( _gen_texture_() )
{
    if ( !tex_node.isObject() )
        throw std::runtime_error( "texture node is not KV" );

    NamedValueSet& tex_kv = tex_node.getDynamicObject()->getProperties();
    {
        Result re = _validate_keys_( tex_kv );
        if (!re)
            throw ConfigParseError( re.getErrorMessage() );
    }

    //
    // create OpenGL texture object
    //
    if ( !fromString( tex_kv[KEY_TYPE], m_type ) )
        throw ConfigParseError( "failed to parse type from " + tex_kv[KEY_TYPE].toString() );
    glBindTexture( m_type, m_texture );

    //
    // load properties
    //
    bool mag_linear = false;
    if ( tex_kv.contains( KEY_MAG_LINEAR ) )
        mag_linear = bool(tex_kv[KEY_MAG_LINEAR]);

    if (mag_linear) set_mag_filter( TFGL_TEXTURE_LINEAR );
    else            set_mag_filter( TFGL_TEXTURE_NEAREST );

    if ( tex_kv.contains( KEY_WRAP_S ) )
    {
        GLTextureWrap wrap_s;
        if ( !fromString( tex_kv[KEY_WRAP_S], wrap_s ) )
            throw ConfigParseError( "failed to parse texture S wrap from " + tex_kv[KEY_WRAP_S].toString() );
        set_wrap_s( wrap_s );
    }

    if ( tex_kv.contains( KEY_WRAP_T ) )
    {
        GLTextureWrap wrap_t;
        if ( !fromString( tex_kv[KEY_WRAP_T], wrap_t ) )
            throw ConfigParseError( "failed to parse texture T wrap from " + tex_kv[KEY_WRAP_T].toString() );
        set_wrap_t( wrap_t );
    }

    TextureImageSoloChannelPolicy pol_solo = TEXTURE_IMAGE_SOLO_AS_LUMINANCE;
    TextureImageDualChannelPolicy pol_dual = TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA;
    TextureImageIntDataPolicy     pol_int  = TEXTURE_IMAGE_INT_TO_FLOAT;

    if ( tex_kv.contains( KEY_POL_SOLO ) && !fromString( tex_kv[KEY_POL_SOLO], pol_solo ) )
        throw ConfigParseError( "failed to parse solo policy from " + tex_kv[KEY_POL_SOLO].toString() );

    if ( tex_kv.contains( KEY_POL_DUAL ) && !fromString( tex_kv[KEY_POL_DUAL], pol_dual ) )
        throw ConfigParseError( "failed to parse dual policy from " + tex_kv[KEY_POL_DUAL].toString() );

    if ( tex_kv.contains( KEY_POL_INT ) && !fromString( tex_kv[KEY_POL_INT], pol_int ) )
        throw ConfigParseError( "failed to parse dual policy from " + tex_kv[KEY_POL_INT].toString() );

    //
    // load image
    //
    uint32 num_gen_mipmap = 3;

    // multiple texture for mipmapped 2D texture or cube map texture
    if ( tex_kv[KEY_IMG].isArray() )
    {
        Array<var>* image_name_nodes = tex_kv[KEY_IMG].getArray();

        // load images specified for each mipmap
        if (m_type == TFGL_TEXTURE_2D)
        {
            num_gen_mipmap = image_name_nodes->size() - 1;

            glTexParameteri( m_type, GL_TEXTURE_BASE_LEVEL, 0 );
            glTexParameteri( m_type, GL_TEXTURE_MAX_LEVEL,  num_gen_mipmap );

            for (int level = 0; level < image_name_nodes->size(); level++)
            {
                String img_name = (*image_name_nodes)[level].toString();
                Image* img      = ImageManager::getInstance()->get_image( img_name );

                _gl_tex_image_( m_type, level, img->get_texture_compatible_2d( pol_solo, pol_dual, pol_int ) );
                _check_error_unbind_( m_type, "assigning 2D texture data for mipmap level " + String( level ) );
            }
        }

        // load six images for cube map
        else if (m_type == TFGL_TEXTURE_CUBE)
        {
            // parse mipmap level
            if ( tex_kv.contains( KEY_MIPMAP ) && !fromString( tex_kv[KEY_MIPMAP], num_gen_mipmap ) )
                throw ConfigParseError( "failed to parse mipmap level from " + tex_kv[KEY_MIPMAP].toString() );

            glTexParameteri( m_type, GL_TEXTURE_BASE_LEVEL, 0 );
            glTexParameteri( m_type, GL_TEXTURE_MAX_LEVEL,  num_gen_mipmap );

            // load images
            if (image_name_nodes->size() != 6)
                throw ConfigParseError( "invalid number of image for 2D cube map texture: " + String( image_name_nodes->size() ) );

            Image* img_xp = ImageManager::getInstance()->get_image( (*image_name_nodes)[0] );
            Image* img_xn = ImageManager::getInstance()->get_image( (*image_name_nodes)[1] );
            Image* img_yp = ImageManager::getInstance()->get_image( (*image_name_nodes)[2] );
            Image* img_yn = ImageManager::getInstance()->get_image( (*image_name_nodes)[3] );
            Image* img_zp = ImageManager::getInstance()->get_image( (*image_name_nodes)[4] );
            Image* img_zn = ImageManager::getInstance()->get_image( (*image_name_nodes)[5] );

            // set texture image
            _gl_tex_image_( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, img_xp->get_texture_compatible_2d( pol_solo, pol_dual, pol_int ) );
            _check_error_unbind_( m_type, "assigning 2D cube texture x+ image data" );

            _gl_tex_image_( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, img_xn->get_texture_compatible_2d( pol_solo, pol_dual, pol_int ) );
            _check_error_unbind_( m_type, "assigning 2D cube texture x- image data" );

            _gl_tex_image_( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, img_yp->get_texture_compatible_2d( pol_solo, pol_dual, pol_int ) );
            _check_error_unbind_( m_type, "assigning 2D cube texture y+ image data" );

            _gl_tex_image_( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, img_yn->get_texture_compatible_2d( pol_solo, pol_dual, pol_int ) );
            _check_error_unbind_( m_type, "assigning 2D cube texture y- image data" );

            _gl_tex_image_( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, img_zp->get_texture_compatible_2d( pol_solo, pol_dual, pol_int ) );
            _check_error_unbind_( m_type, "assigning 2D cube texture z+ image data" );

            _gl_tex_image_( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, img_zn->get_texture_compatible_2d( pol_solo, pol_dual, pol_int ) );
            _check_error_unbind_( m_type, "assigning 2D cube texture z- image data" );

            // generate mipmap
            if (num_gen_mipmap > 0)
            {
                glGenerateMipmap( m_type );
                _check_error_unbind_( m_type, "generating 2D cube texture mipmaps" );
            }
        }
        else
        {
            throw ConfigParseError( "texture type is " + tex_kv[KEY_TYPE].toString() + ", but data is 2D images which assume mipmapped texture 2D " );
        }
    }

    // single image for 2D, 2D array or 3D texture
    else
    {
        // parse mipmap level
        if ( tex_kv.contains( KEY_MIPMAP ) && !fromString( tex_kv[KEY_MIPMAP], num_gen_mipmap ) )
            throw ConfigParseError( "failed to parse mipmap level from " + tex_kv[KEY_MIPMAP].toString() );

        glTexParameteri( m_type, GL_TEXTURE_BASE_LEVEL, 0 );
        glTexParameteri( m_type, GL_TEXTURE_MAX_LEVEL,  num_gen_mipmap );

        // get image data
        Image* img = ImageManager::getInstance()->get_image( tex_kv[KEY_IMG] );

        // set texture image
        if (m_type == TFGL_TEXTURE_2D)
        {
            _gl_tex_image_( m_type, 0, img->get_texture_compatible_2d( pol_solo, pol_dual, pol_int ) );
            _check_error_unbind_( m_type, "assigning 2D texture image data" );
        }
        else if (m_type == TFGL_TEXTURE_2D_ARRAY || m_type == TFGL_TEXTURE_3D)
        {
            // get number of slices for 2D array or 3D texture
            GLsizei slices = 0;
            if ( !tex_kv.contains( KEY_SLICES ) )
                throw ConfigParseError( "2D array or 3D texture don't have property " KEY_SLICES );

            if ( !fromString( tex_kv[KEY_SLICES], slices ) )
                throw ConfigParseError( "failed to parse slice number from " + tex_kv[KEY_SLICES].toString() );

            _gl_tex_image_( m_type, 0, img->get_texture_compatible_3d( slices, pol_solo, pol_dual, pol_int ) );
            _check_error_unbind_( m_type, "assigning 2D array or 3D texture image data" );
        }

        // generate mipmap
        if (num_gen_mipmap > 0)
        {
            glGenerateMipmap( m_type );
            _check_error_unbind_( m_type, "generating texture mipmaps" );
        }
    }

    // set filter params
    bool min_linear = true;
    if ( tex_kv.contains( KEY_MIN_LINEAR ) )
        min_linear = bool(tex_kv[KEY_MIN_LINEAR]);

    GLTextureFilter min_filter;
    if (num_gen_mipmap > 0)
    {
        bool mipmap_linear = true;
        if ( tex_kv.contains( KEY_MIPMAP_LINEAR ) )
            mipmap_linear = bool(tex_kv[KEY_MIPMAP_LINEAR]);

        if (min_linear)
        {
            if (mipmap_linear) min_filter = TFGL_TEXTURE_LINEAR_MIPMAP_LINEAR;
            else               min_filter = TFGL_TEXTURE_LINEAR_MIPMAP_NEAREST;
        }
        else
        {
            if (mipmap_linear) min_filter = TFGL_TEXTURE_NEAREST_MIPMAP_LINEAR;
            else               min_filter = TFGL_TEXTURE_NEAREST_MIPMAP_NEAREST;
        }
    }
    else
    {
        if (min_linear) min_filter = TFGL_TEXTURE_LINEAR;
        else            min_filter = TFGL_TEXTURE_NEAREST;
    }

    set_min_filter( min_filter );

    glBindTexture( m_type, 0 );
}

Texture::~Texture()
{
    if (m_texture)
        glDeleteTextures( 1, &m_texture );
}

GLuint Texture::get_current_bound_texture( GLTextureType type ) noexcept
{
    GLenum pname  = -1;
    GLint  result = -1;

    switch (type)
    {
    case TFGL_TEXTURE_2D:       pname = GL_TEXTURE_BINDING_2D;       break;
    case TFGL_TEXTURE_2D_ARRAY: pname = GL_TEXTURE_BINDING_2D_ARRAY; break;
    case TFGL_TEXTURE_CUBE:     pname = GL_TEXTURE_BINDING_CUBE_MAP; break;
    case TFGL_TEXTURE_3D:       pname = GL_TEXTURE_BINDING_3D;       break;
    default: abort();
    }

    glGetIntegerv( pname, &result );

    return GLuint( result );
}

} // namespace treeface
