#ifndef TREEFACE_POST_PROCESS_H
#define TREEFACE_POST_PROCESS_H

#include <treecore/RefCountObject.h>

namespace treeface
{

class PostProcess: public treecore::RefCountObject
{
public:
    PostProcess();

protected:
    struct Guts;
    Guts* m_guts;
};

} // namespace treeface

#endif // TREEFACE_POST_PROCESS_H
