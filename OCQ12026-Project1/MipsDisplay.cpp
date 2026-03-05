#define OLC_PGE_APPLICATION

#include <chrono>
#include "MipsDisplay.hpp"
#include <memory>

MipsDisplay::MipsDisplay()
{ sAppName = "MIPS Virtual FrameBuffer"; }

MipsDisplay::~MipsDisplay()
{
    std::cout << "1. Stopping engine ..." << std::endl;
    StopEngine();
    std::cout << "2. Stopping engine ..." << std::endl;
}

bool MipsDisplay::OnUserCreate()
{ 
    return true; 
}

bool MipsDisplay::OnUserDestroy()
{
    return false;
}

bool MipsDisplay::OnUserUpdate(float fElapsedTime)
{   
    if (GetKey(olc::Key::W).bPressed || GetKey(olc::Key::UP).bPressed)
        last_key = 1;
    else if (GetKey(olc::Key::S).bPressed || GetKey(olc::Key::DOWN).bPressed)
        last_key = 2;
    else if (GetKey(olc::Key::A).bPressed || GetKey(olc::Key::LEFT).bPressed)
        last_key = 3;
    else if (GetKey(olc::Key::D).bPressed || GetKey(olc::Key::RIGHT).bPressed)
        last_key = 4;
    else if (GetKey(olc::Key::SPACE).bPressed)
        last_key = 5;
    else
        last_key = 0;

    return running;
}

void MipsDisplay::Sleep(int ms)
{  
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));  
}

void MipsDisplay::Flush()
{
    for (int y = 0; y < SCREEN_H; y++)
        for (int x = 0; x < SCREEN_W; x++)
            Draw(x, y, olc::Pixel(vram[y * SCREEN_W + x]));
}

void MipsDisplay::RunEngine()
{
    thread_ = std::thread([this]() {
        running = true;

        if (Construct(SCREEN_W, SCREEN_H, PIXEL_SIZE, PIXEL_SIZE)) {
            Start();
        }

        running = false;
    });
}

void MipsDisplay::StopEngine()
{
    running = false;

    if (thread_.joinable()) {
        thread_.join();
    }
    olc_Terminate();
}

//CODIGO NO DEL TEMPLATE
void MipsDisplay::SetPixel(int x, int y, uint32_t color)
{
    if (x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H)
        vram[y * SCREEN_W + x] = color;
}

void MipsDisplay::Clear(uint32_t color)
{
    for (int i = 0; i < SCREEN_W * SCREEN_H; i++)
        vram[i] = color;
}

