#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <TlHelp32.h>

#include <cstdint>
#include <string_view>

class Memory
{
private:
    std::uintptr_t procId_ = 0;
    void* processHandle_ = nullptr;

public:
    explicit Memory(const std::string_view processName) noexcept
    {
        const auto snap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snap == INVALID_HANDLE_VALUE) return;

        ::PROCESSENTRY32 entry = { .dwSize = sizeof(::PROCESSENTRY32) };

        if (::Process32First(snap, &entry))
        {
            do {
                if (processName == entry.szExeFile)
                {
                    procId_ = entry.th32ProcessID;
                    processHandle_ = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId_);
                    break;
                }
            } while (::Process32Next(snap, &entry));
        }

        ::CloseHandle(snap);
    }

    ~Memory()
    {
        if (processHandle_)
            ::CloseHandle(processHandle_);
    }

    Memory(const Memory&) = delete;
    Memory& operator=(const Memory&) = delete;

    [[nodiscard]] std::uintptr_t GetModuleAddress(const std::string_view moduleName) const noexcept
    {
        const auto snap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId_);
        if (snap == INVALID_HANDLE_VALUE) return 0;

        ::MODULEENTRY32 entry = { .dwSize = sizeof(::MODULEENTRY32) };
        std::uintptr_t address = 0;

        if (::Module32First(snap, &entry))
        {
            do {
                if (moduleName == entry.szModule)
                {
                    address = reinterpret_cast<std::uintptr_t>(entry.modBaseAddr);
                    break;
                }
            } while (::Module32Next(snap, &entry));
        }

        ::CloseHandle(snap);
        return address;
    }

    template <typename T>
    [[nodiscard]] constexpr T Read(const std::uintptr_t address) const noexcept
    {
        T value = { };
        ::ReadProcessMemory(processHandle_, reinterpret_cast<const void*>(address), &value, sizeof(T), nullptr);
        return value;
    }

    template <typename T>
    constexpr void Write(const std::uintptr_t address, const T& value) const noexcept
    {
        ::WriteProcessMemory(processHandle_, reinterpret_cast<void*>(address), &value, sizeof(T), nullptr);
    }
};