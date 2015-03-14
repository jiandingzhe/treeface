#ifndef TREEFACE_COMMON_H
#define TREEFACE_COMMON_H

#define STRINGIFY(content) #content
#define EXPAND_AND_STRINGIFY(input) STRINGIFY(input)

// wrapper macro for namespace
// in avoid of some stupid IDE would disturb code indentation
#define TREEFACE_NAMESPACE_BEGIN namespace treeface {
#define TREEFACE_NAMESPACE_END }

#endif // TREEFACE_COMMON_H
