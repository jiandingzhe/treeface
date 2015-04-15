#ifndef TREEFACE_PROGRAM_MANAGER_H
#define TREEFACE_PROGRAM_MANAGER_H

#include "treeface/common.h"

#include <treejuce/Singleton.h>
#include <treejuce/String.h>

TREEFACE_NAMESPACE_BEGIN

class Program;

class ProgramManager
{
public:
    JUCE_DECLARE_NON_COPYABLE(ProgramManager);
    JUCE_DECLARE_NON_MOVABLE(ProgramManager);
    juce_DeclareSingleton(ProgramManager, false);

    Program* get_program(const treejuce::String& name_vertex, const treejuce::String& name_fragment);
    bool program_is_cached(const treejuce::String& name_vertex, const treejuce::String& name_fragment);
    bool release_program_hold(const treejuce::String& name_vertex, const treejuce::String& name_fragment);

protected:
    struct Impl;

    ProgramManager();
    ~ProgramManager();

    Impl* m_impl = nullptr;
};

TREEFACE_NAMESPACE_END

#endif // TREEFACE_PROGRAM_MANAGER_H
