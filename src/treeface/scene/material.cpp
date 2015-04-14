#include "treeface/scene/material.h"

#include "treeface/gl/program.h"
#include "treeface/gl/texture.h"

#include <treejuce/Array.h>
#include <treejuce/Object.h>
#include <treejuce/Holder.h>

using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct TextureLayer
{
    Holder<Texture> gl_texture;
    GLint texture_unit_idx;
    GLint program_sampler_idx;
    String program_sampler_name;
};

struct Material::Impl
{

};


Material::Material()
{
    // TODO: create material here?
}

Material::~Material()
{
    if (m_impl)
        delete m_impl;
}

TREEFACE_NAMESPACE_END
