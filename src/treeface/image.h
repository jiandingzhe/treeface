#ifndef TREEFACE_IMAGE_H
#define TREEFACE_IMAGE_H

#include "treeface/common.h"
#include "treeface/gl/imageref.h"

#include <treejuce/Object.h>

#include <FreeImage.h>

#define GLEW_STATIC
#include <GL/glew.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

treejuce::Result convert_fi_image_info_to_gl(FIBITMAP* fi_img, GLenum& gl_format, GLenum& gl_type);

class Image: public treejuce::Object
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
    Image(FIBITMAP* fi_img);

    // copy
    Image(const Image& other);
    Image& operator = (const Image& other);

    // move
    Image(Image&& other);
    Image& operator = (Image&& other);

    virtual ~Image();

    /**
     * @brief set the underlying FreeImage image object. The original object will
     *        be destroyed.
     *
     * @param new_img: the new FreeImage image object. It will be owned by Image,
     *        and will be destroyed together when Image is destroyed.
     */
    void set_image(FIBITMAP* new_img);

    /**
     * @brief set the underlying FreeImage image object and get the original one.
     * @param new_img: the new FreeImage image object. It will bw owned by Image,
     *        and will be destroyed together when Image is destroyed.
     * @return: the original FreeImage image object.
     */
    FIBITMAP* exchange_image(FIBITMAP* new_img);



    ImageRef get_image_ref() const NOEXCEPT;

protected:
    GLenum m_gl_format = 0;
    GLenum m_gl_type = 0;
    FIBITMAP* m_fi_img = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_IMAGE_H
