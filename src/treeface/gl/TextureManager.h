#ifndef TREEFACE_TEXTURE_MANAGER_H
#define TREEFACE_TEXTURE_MANAGER_H

#include <treecore/Identifier.h>
#include <treecore/RefCountObject.h>
#include <treecore/RefCountSingleton.h>

namespace treecore {
class var;
} // namespace treecore

namespace treeface
{

class Texture;

class TextureManager: public treecore::RefCountObject, public treecore::RefCountSingleton<TextureManager>
{
    friend class treecore::RefCountSingleton<TextureManager>;

public:
    TREECORE_DECLARE_NON_COPYABLE( TextureManager );
    TREECORE_DECLARE_NON_MOVABLE( TextureManager );

    Texture* build_texture( const treecore::Identifier& name, const treecore::var& data );
    Texture* get_texture( const treecore::Identifier& name );
    bool     has_texture( const treecore::Identifier& name );
    bool     release_texture_hold( const treecore::Identifier& name );

protected:
    TextureManager();
    virtual ~TextureManager();

    struct Guts;
    Guts* m_guts;
};

} // namespace treeface

#endif // TREEFACE_TEXTURE_MANAGER_H
