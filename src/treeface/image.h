#ifndef TREEFACE_IMAGE_H
#define TREEFACE_IMAGE_H

#include "treeface/common.h"
#include "treeface/enums.h"
#include "treeface/gl/imageref.h"

#include <treecore/RefCountObject.h>

#ifdef TREECORE_OS_WINDOWS
#  include <Windows.h>
#endif

#include <FreeImage.h>

#define GLEW_STATIC
#include <GL/glew.h>

namespace treecore {
class MemoryBlock;
class Result;
} // namespace treecore

namespace treeface {

void intepret_fi_image_format( FIBITMAP* fi_img, treecore::uint8& result_num_channel, GLImageDataType& result_gl_type );

class Image: public treecore::RefCountObject
{
public:
    Image();

    /**
     * @brief create a new Image object specifying the underlying FreeImage image
     *        object.
     *
     * @param fi_img: an FreeImage image object. It will be owned by Image, and
     *        will be destroyed together when Image is destroyed.
     */
    Image( FIBITMAP* fi_img );

    ///
    /// \brief Image
    /// \param memory
    ///
    Image( treecore::MemoryBlock& memory );

    // copy
    Image( const Image& other );
    Image& operator =( const Image& other );

    TREECORE_DECLARE_NON_MOVABLE( Image );

    virtual ~Image();

    /**
     * @brief set the underlying FreeImage image object. The original object will
     *        be destroyed.
     *
     * @param new_img: the new FreeImage image object. It will be owned by Image,
     *        and will be destroyed together when Image is destroyed.
     */
    void set_image( FIBITMAP* new_img );

    int32           get_num_channel() const noexcept { return m_num_channel; }
    GLImageDataType get_data_type()   const noexcept { return m_gl_type; }
    int32           get_width() const noexcept       { return (int32) FreeImage_GetWidth( m_fi_img ); }
    int32           get_height() const noexcept      { return (int32) FreeImage_GetHeight( m_fi_img ); }

    /**
     * @brief set the underlying FreeImage image object and get the original one.
     * @param new_img: the new FreeImage image object. It will bw owned by Image,
     *        and will be destroyed together when Image is destroyed.
     * @return: the original FreeImage image object.
     */
    FIBITMAP* exchange_image( FIBITMAP* new_img );

    TextureCompatibleImageRef get_texture_compatible_2d( TextureImageSoloChannelPolicy solo_pol = TEXTURE_IMAGE_SOLO_AS_LUMINANCE,
                                                         TextureImageDualChannelPolicy dual_pol = TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA,
                                                         TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleImageRef get_texture_compatible_2d_solo( TextureImageSoloChannelPolicy solo_pol = TEXTURE_IMAGE_SOLO_AS_LUMINANCE,
                                                              TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleImageRef get_texture_compatible_2d_dual( TextureImageDualChannelPolicy dual_pol = TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA,
                                                              TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleImageRef get_texture_compatible_2d_triple( TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleImageRef get_texture_compatible_2d_quad( TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleImageArrayRef get_texture_compatible_2d_array( int32 num_frame,
                                                                    TextureImageSoloChannelPolicy solo_pol = TEXTURE_IMAGE_SOLO_AS_LUMINANCE,
                                                                    TextureImageDualChannelPolicy dual_pol = TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA,
                                                                    TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleImageArrayRef get_texture_compatible_2d_array_solo( int32 num_frame,
                                                                         TextureImageSoloChannelPolicy solo_pol = TEXTURE_IMAGE_SOLO_AS_LUMINANCE,
                                                                         TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleImageArrayRef get_texture_compatible_2d_array_dual( int32 num_frame,
                                                                         TextureImageDualChannelPolicy dual_pol = TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA,
                                                                         TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleImageArrayRef get_texture_compatible_2d_array_triple( int32 num_frame,
                                                                           TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleImageArrayRef get_texture_compatible_2d_array_quad( int32 num_frame,
                                                                         TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleVoxelBlockRef get_texture_compatible_3d( int32 num_slice,
                                                              TextureImageSoloChannelPolicy solo_pol = TEXTURE_IMAGE_SOLO_AS_LUMINANCE,
                                                              TextureImageDualChannelPolicy dual_pol = TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA,
                                                              TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleVoxelBlockRef get_texture_compatible_3d_solo( int32 num_slice,
                                                                   TextureImageSoloChannelPolicy solo_pol = TEXTURE_IMAGE_SOLO_AS_LUMINANCE,
                                                                   TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleVoxelBlockRef get_texture_compatible_3d_dual( int32 num_slice,
                                                                   TextureImageDualChannelPolicy dual_pol = TEXTURE_IMAGE_DUAL_AS_LUMINANCE_ALPHA,
                                                                   TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleVoxelBlockRef get_texture_compatible_3d_triple( int32 num_slice,
                                                                     TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

    TextureCompatibleVoxelBlockRef get_texture_compatible_3d_quad( int32 num_slice,
                                                                   TextureImageIntDataPolicy int_pol  = TEXTURE_IMAGE_INT_TO_FLOAT ) const;

protected:
    treecore::uint8 m_num_channel = 0;
    GLImageDataType m_gl_type;
    FIBITMAP*       m_fi_img = nullptr;
};

} // namespace treeface

#endif // TREEFACE_IMAGE_H
