#pragma once
#include "../../memory/memory.h"
#include "../../memory/offsets.h"
#include "../../menu/data/components.h"

class PlayerModifiers {
public:
    static void run(const Memory& mem, std::uintptr_t base) noexcept;
};