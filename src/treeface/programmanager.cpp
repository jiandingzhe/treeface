#include "treeface/gl/program.h"
#include "treeface/packagemanager.h"
#include "treeface/programmanager.h"

#include <treejuce/HashMap.h>
#include <treejuce/Holder.h>
#include <treejuce/ScopedPointer.h>
#include <treejuce/StringRef.h>
#include <treejuce/ContainerDeletePolicy.h>

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

juce_ImplementSingleton(ProgramManager);

struct ProgramManager::Impl
{
    HashMap<String, Holder<Program> > programs;
};

inline String _format_key_(const treejuce::String& name_vertex, const treejuce::String& name_fragment)
{
    return name_vertex + "|||" + name_fragment;
}

Program* ProgramManager::get_program(const treejuce::String& name_vertex, const treejuce::String& name_fragment)
{
    String key = _format_key_(name_vertex, name_fragment);

    if (m_impl->programs.contains(key))
    {
        return m_impl->programs[key];
    }
    else
    {
        int64 src_vertex_size = -1;
        ScopedPointer<uint8> src_vertex = PackageManager::getInstance()->get_item_data(name_vertex, src_vertex_size);
        if (!src_vertex)
            return nullptr;

        int64 src_frag_size = -1;
        ScopedPointer<uint8> src_frag = PackageManager::getInstance()->get_item_data(name_fragment, src_frag_size);
        if (!src_frag)
            return nullptr;

        Program* program = new Program();
        Result program_re = program->init((char*)src_vertex.get(), (char*)src_frag.get());

        if (!program_re)
            die("%s", program_re.getErrorMessage().toRawUTF8());

        m_impl->programs.set(key, program);

        return program;
    }
}

bool ProgramManager::program_is_cached(const treejuce::String& name_vertex, const treejuce::String& name_fragment)
{
    String key = _format_key_(name_vertex, name_fragment);
    return m_impl->programs.contains(key);
}

bool ProgramManager::release_program_hold(const treejuce::String& name_vertex, const treejuce::String& name_fragment)
{
    String key = _format_key_(name_vertex, name_fragment);
    if (m_impl->programs.contains(key))
    {
        m_impl->programs.remove(key);
        return true;
    }
    else
    {
        return false;
    }
}


TREEFACE_NAMESPACE_END
