#ifndef TREEFACE_IMAGE_H
#define TREEFACE_IMAGE_H

#include "treeface/gl/imageref.h"

#include <treejuce/Object.h>

#include <FreeImage.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
class Result;
TREEFACE_JUCE_NAMESPACE_END

TREEFACE_NAMESPACE_BEGIN

treejuce::Result convert_fi_image_info_to_gl(FIBITMAP* fi_img, ImageRef& info);

class Image: public treejuce::Object
{
public:
    Image();
    Image(FIBITMAP* fi_img);

    Image(Image&& other);
    Image& operator = (Image&& other);

    virtual ~Image();

    inline ImageRef get_image_ref() const NOEXCEPT
    {
        return m_img_ref;
    }

protected:
    ImageRef m_img_ref{0, 0, 0, 0, nullptr};
    FIBITMAP* m_fi_img = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_IMAGE_H
