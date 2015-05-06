#include "treeface/gl/programmanager.h"

#include "treeface/gl/program.h"
#include "treeface/gl/sceneprogram.h"

#include "treeface/packagemanager.h"

#include <treejuce/HashMap.h>
#include <treejuce/Holder.h>
#include <treejuce/MemoryBlock.h>
#include <treejuce/ScopedPointer.h>
#include <treejuce/StringRef.h>
#include <treejuce/ContainerDeletePolicy.h>


using namespace treejuce;

TREEFACE_NAMESPACE_BEGIN

struct ProgramManager::Impl
{
    HashMap<String, Holder<Program> > programs;
};

inline String _format_key_(const treejuce::String& name_vertex, const treejuce::String& name_fragment)
{
    return name_vertex + "|||" + name_fragment;
}

Result ProgramManager::get_program(const treejuce::String& name_vertex, const treejuce::String& name_fragment, bool scene_additive, Program** program_pp)
{
    String key = _format_key_(name_vertex, name_fragment);

    if (m_impl->programs.contains(key))
    {
        *program_pp = m_impl->programs[key];
        return Result::ok();
    }
    else
    {
        MemoryBlock src_vertex;
        Result re_src_vertex = PackageManager::getInstance()->get_item_data(name_vertex, src_vertex);
        if (!re_src_vertex)
        {
            *program_pp = nullptr;
            return Result::fail("failed to get vertex shader source code:\n" +
                                re_src_vertex.getErrorMessage());
        }

        MemoryBlock src_frag;
        Result re_src_frag = PackageManager::getInstance()->get_item_data(name_fragment, src_frag);
        if (!re_src_frag)
        {
            *program_pp = nullptr;
            return Result::fail("failed to get fragment shader source code:\n" +
                                re_src_frag.getErrorMessage());
        }

        Holder<Program> program = scene_additive ? new SceneProgram() : new Program();
        Result program_re = program->build((char*)src_vertex.getData(), (char*)src_frag.getData());

        if (!program_re)
        {
            *program_pp = nullptr;
            return Result::fail("failed to create program using \""+name_vertex+"\" and \""+name_fragment+"\":\n"+
                                program_re.getErrorMessage());
        }

        m_impl->programs.set(key, program);
        *program_pp = program.get();

        return Result::ok();
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

ProgramManager::ProgramManager()
    : m_impl(new ProgramManager::Impl())
{
}

ProgramManager::~ProgramManager()
{
    if (m_impl)
        delete m_impl;
}


TREEFACE_NAMESPACE_END
