#include "treeface/materialmanager.h"

#include "treeface/scene/material.h"
#include "treeface/packagemanager.h"

#include <treejuce/HashMap.h>
#include <treejuce/Holder.h>
#include <treejuce/JSON.h>
#include <treejuce/String.h>

TREEFACE_JUCE_NAMESPACE_BEGIN
template <>
struct ContainerDeletePolicy<uint8>
{
    static void destroy (uint8* ptr)
    {
        free(ptr);
    }
};
TREEFACE_JUCE_NAMESPACE_END


using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

juce_ImplementSingleton(MaterialManager);

struct MaterialManager::Impl
{
    HashMap<String, Holder<Material> > materials;
};

Result MaterialManager::get_material(const String& name, Material** mat)
{
    if (m_impl->materials.contains(name))
    {
        *mat = m_impl->materials[name].get();
        return Result::ok();
    }

    ArrayRef<uint8> mat_data = PackageManager::getInstance()->get_item_data(name);
    if (!mat_data.get_data())
    {
        *mat = nullptr;
        return Result::fail("failed to get material config from package manager using name \""+name+"\"");
    }

    ScopedPointer<uint8> mat_data_scope(mat_data.get_data());

    var mat_root_node;
    Result json_re = JSON::parse((char*)mat_data.get_data(), mat_root_node);
    if (!json_re)
    {
        *mat = nullptr;
        return Result::fail(String("failed to parse material JSON content for \"") + name + String("\":\n") +
                            json_re.getErrorMessage() + String("\n") +
                            String((const char*)mat_data.get_data())
                            );
    }

    Holder<Material> mat_holder = new Material();
    Result mat_re = mat_holder->build(mat_root_node);
    if (!mat_re)
    {
        *mat = nullptr;
        return Result::fail(String("failed to build material using JSON content for \"") + name + String("\":\n") +
                            mat_re.getErrorMessage() + String("\n") +
                            String((const char*)mat_data.get_data())
                            );
    }

    *mat = mat_holder.get();
    m_impl->materials.set(name, mat_holder);
    return Result::ok();
}

treejuce::Result MaterialManager::get_material(const treejuce::String& name, treejuce::Holder<Material>& result)
{
    Material* mat_ptr = nullptr;
    Result re = get_material(name, &mat_ptr);
    result = mat_ptr;
    return re;
}

bool MaterialManager::material_is_cached(const treejuce::String& name)
{
    return m_impl->materials.contains(name);
}

bool MaterialManager::release_material_hold(const treejuce::String& name)
{
    if (m_impl->materials.contains(name))
    {
        m_impl->materials.remove(name);
        return true;
    }
    else
    {
        return false;
    }
}

MaterialManager::MaterialManager()
    : m_impl(new Impl)
{
}

MaterialManager::~MaterialManager()
{
    if (m_impl)
        delete m_impl;
}

TREEFACE_NAMESPACE_END
