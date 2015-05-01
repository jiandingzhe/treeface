#ifndef TREEFACE_VISUAL_ITEM_H
#define TREEFACE_VISUAL_ITEM_H

#include "treeface/common.h"

#include <treejuce/Object.h>

TREEFACE_NAMESPACE_BEGIN

class VisualItem: public treejuce::Object
{
public:
    VisualItem();
    virtual ~VisualItem();

    virtual void draw() = 0;

private:
    struct Impl;
    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_VISUAL_ITEM_H
