#include "treeface/gl/TextureManager.h"

#include "treeface/base/PackageManager.h"
#include "treeface/gl/Texture.h"
#include "treeface/graphics/ImageManager.h"
#include "treeface/misc/Errors.h"

#include <treecore/HashMap.h>
#include <treecore/RefCountHolder.h>

namespace treeface
{

typedef treecore::HashMap<treecore::Identifier, treecore::RefCountHolder<Texture> > TextureMap;

struct TextureManager::Guts
{
    TextureMap textures;
};

TextureManager::TextureManager(): m_guts( new Guts )
{}

TextureManager::~TextureManager()
{
    delete m_guts;
}

Texture* TextureManager::build_texture( const treecore::Identifier& name, const treecore::var& data )
{
    Texture* tex = new Texture( data );
    m_guts->textures.set( name, tex );
    return tex;
}

Texture* TextureManager::get_texture( const treecore::Identifier& name )
{
    TextureMap::Iterator it( m_guts->textures );
    if ( m_guts->textures.select( name, it ) )
    {
        return it.value().get();
    }
    else
    {
        treecore::var tex_root_node = PackageManager::getInstance()->get_item_json( name );
        if (!tex_root_node)
            throw ConfigParseError( "no texture named " + name.toString() );
        return build_texture( name, tex_root_node );
    }
}

bool TextureManager::has_texture( const treecore::Identifier& name )
{
    return m_guts->textures.contains( name );
}

bool TextureManager::release_texture_hold( const treecore::Identifier& name )
{
    return m_guts->textures.remove( name );
}

} // namespace treeface
