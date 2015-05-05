#ifndef TREEFACE_PROGRAM_MANAGER_H
#define TREEFACE_PROGRAM_MANAGER_H

#include "treeface/common.h"

#include <treejuce/Result.h>
#include <treejuce/Object.h>
#include <treejuce/String.h>

TREEFACE_NAMESPACE_BEGIN

class Program;

class ProgramManager: public treejuce::Object
{
public:
    ProgramManager();

    JUCE_DECLARE_NON_COPYABLE(ProgramManager);
    JUCE_DECLARE_NON_MOVABLE(ProgramManager);

    virtual ~ProgramManager();

    treejuce::Result get_program(const treejuce::String& name_vertex, const treejuce::String& name_fragment, bool scene_additive, Program** program_pp);
    bool program_is_cached(const treejuce::String& name_vertex, const treejuce::String& name_fragment);
    bool release_program_hold(const treejuce::String& name_vertex, const treejuce::String& name_fragment);

private:
    struct Impl;

    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_PROGRAM_MANAGER_H
