#pragma once
#include <Windows.h>
#include <TlHelp32.h>
namespace hwbp
{
    inline void SetForThread(HANDLE th, void* addr, int index = 0)
    {
        CONTEXT ctx{};
        ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

        GetThreadContext(th, &ctx);

        (&ctx.Dr0)[index] = (DWORD_PTR)addr;

        // trigger bp
        ctx.Dr7 |= (1 << (index * 2));

        SetThreadContext(th, &ctx);
    }

    inline void ClearForThread(HANDLE th, int index = 0)
    {
        CONTEXT ctx{};
        ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

        GetThreadContext(th, &ctx);

        (&ctx.Dr0)[index] = 0;
        ctx.Dr7 &= ~(1 << (index * 2));


        SetThreadContext(th, &ctx);
    }

    inline void SetAll(void* addr, int index = 0)
    {
        THREADENTRY32 te{ sizeof(te) };
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

        Thread32First(snap, &te);
        do
        {
            if (te.th32OwnerProcessID == GetCurrentProcessId())
            {
                HANDLE th = OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
                if (th)
                {
                    SetForThread(th, addr, index);
                    CloseHandle(th);
                }
            }
        } while (Thread32Next(snap, &te));

        CloseHandle(snap);
    }

    inline void ClearAll(int index = 0)
    {
        THREADENTRY32 te{ sizeof(te) };
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

        Thread32First(snap, &te);
        do
        {
            if (te.th32OwnerProcessID == GetCurrentProcessId())
            {
                HANDLE th = OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
                if (th)
                {
                    ClearForThread(th, index);
                    CloseHandle(th);
                }
            }
        } while (Thread32Next(snap, &te));

        CloseHandle(snap);
    }
}