// This proc.h file is used to share the 'memory' instance across different translation units (.cppP files).
// By including this header, other .cpp files can access the same 'memory' instance for memory operations.
// The 'extern' keyword is used to indicate that the 'memory' instance is declared elsewhere (in this case, in 'Memory.cpp').
// This helps ensure consistent memory access and manipulation throughout the codebase.

#pragma once

#include "../memory/memory.h"
extern Memory memory;