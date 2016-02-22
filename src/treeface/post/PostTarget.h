#ifndef TREEFACE_POST_TARGET_H
#define TREEFACE_POST_TARGET_H

#include <treecore/RefCountObject.h>

namespace treeface
{

class PostSurface;
class PostProcess;

class PostTarget: public treecore::RefCountObject
{
public:
    PostTarget();

    TREECORE_DECLARE_NON_COPYABLE(PostTarget);
    TREECORE_DECLARE_NON_MOVABLE(PostTarget);

    virtual ~PostTarget();

protected:
    struct Guts;
    Guts* m_guts = nullptr;
};

} // namespace treeface

#endif // TREEFACE_POST_TARGET_H
