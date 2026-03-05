#include "easm.h"
#include <memory>
#include "MipsDisplay.hpp"

std::unique_ptr<MipsDisplay> display;

extern "C" __declspec(dllexport)
ErrorCode handleSyscall(uint32_t* regs, void* mem, MemoryMap* mem_map)
{
    unsigned v0 = regs[Register::v0];

    switch (v0)
    {
        case 100:
        {
            if (!display)
                display = std::make_unique<MipsDisplay>();
            display->RunEngine();
            return ErrorCode::Ok;
        }
        case 101:
        {
            int x = regs[Register::a0];
            int y = regs[Register::a1];
            uint32_t color = regs[Register::a2];
            if (display)
                display->SetPixel(x, y, color);
            return ErrorCode::Ok;
        }
        case 102:
        {
            if (display)
                display->RefreshWindow();
            return ErrorCode::Ok;
        }
        case 103:
        {
            uint32_t color = regs[Register::a0];
                if (display)
                    display->Clear(color);
            return ErrorCode::Ok;
        }
        case 104:
        {
            if (display)
            {
                int key = display->GetLastKey();
                printf("Key: %d\n", key);
                fflush(stdout);
                regs[Register::v0] = key;
            }
            else
                regs[Register::v0] = 0;
            return ErrorCode::Ok;
        }
        case 105:
        {
            if (display) {
                display->StopEngine();
                display.reset();
            }
            return ErrorCode::Ok;
        }
        default:
            return ErrorCode::SyscallNotImplemented;
    }
}
