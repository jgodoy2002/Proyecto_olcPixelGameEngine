#pragma once

#include "olcPixelGameEngine.h"
#include <thread>
#include <cstdint>
#include <atomic>

class MipsDisplay : public olc::PixelGameEngine
{
public:

    static constexpr const int SCREEN_W = 256;
    static constexpr const int SCREEN_H = 128;
    static constexpr const int PIXEL_SIZE = 4;

    MipsDisplay();
    ~MipsDisplay();
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
    bool OnUserDestroy() override;
    void Flush();
    void RunEngine();
    void StopEngine();
    void Sleep(int ms);
    void RefreshWindow() {
        Flush();
    }
    int GetLastKey() const
    { return last_key; }

    bool IsRunning() const
    { return running; }
    //NUEVO NO PARTE DEL TEMPLATE
    void SetPixel(int x, int y, uint32_t color);
    void Clear(uint32_t color);

private:
    std::atomic<bool> running = false;
    int last_key = 0;
    uint32_t vram[SCREEN_W * SCREEN_H];
    std::thread thread_;
};
