/*
  ==============================================================================

   This file is part of the juce_core module of the JUCE library.
   Copyright (c) 2013 - Raw Material Software Ltd.

   Permission to use, copy, modify, and/or distribute this software for any purpose with
   or without fee is hereby granted, provided that the above copyright notice and this
   permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
   TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
   NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
   DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
   IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

   ------------------------------------------------------------------------------

   NOTE! This permissive ISC license applies ONLY to files within the juce_core module!
   All other JUCE modules are covered by a dual GPL/commercial license, so if you are
   using any other modules, be sure to check that you also comply with their license.

   For more details, visit www.juce.com

  ==============================================================================
*/

#include "treejuce/Result.h"

TREEFACE_JUCE_NAMESPACE_BEGIN

Result::Result() NOEXCEPT {}

Result::Result (const String& message) NOEXCEPT
    : errorMessage (message)
{
}

Result::Result (const Result& other)
    : errorMessage (other.errorMessage)
{
}

Result& Result::operator= (const Result& other)
{
    errorMessage = other.errorMessage;
    return *this;
}

#if JUCE_COMPILER_SUPPORTS_MOVE_SEMANTICS
Result::Result (Result&& other) NOEXCEPT
    : errorMessage (static_cast <String&&> (other.errorMessage))
{
}

Result& Result::operator= (Result&& other) NOEXCEPT
{
    errorMessage = static_cast <String&&> (other.errorMessage);
    return *this;
}
#endif

bool Result::operator== (const Result& other) const NOEXCEPT
{
    return errorMessage == other.errorMessage;
}

bool Result::operator!= (const Result& other) const NOEXCEPT
{
    return errorMessage != other.errorMessage;
}

Result Result::fail (const String& errorMessage) NOEXCEPT
{
    return Result (errorMessage.isEmpty() ? "Unknown Error" : errorMessage);
}

const String& Result::getErrorMessage() const NOEXCEPT
{
    return errorMessage;
}

bool Result::wasOk() const NOEXCEPT         { return errorMessage.isEmpty(); }
Result::operator bool() const NOEXCEPT      { return errorMessage.isEmpty(); }
bool Result::failed() const NOEXCEPT        { return errorMessage.isNotEmpty(); }
bool Result::operator!() const NOEXCEPT     { return errorMessage.isNotEmpty(); }

TREEFACE_JUCE_NAMESPACE_END
