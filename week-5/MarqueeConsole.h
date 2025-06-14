#pragma once
#include "AConsole.h"
#include <thread>
#include <sstream>

class MarqueeConsole : public AConsole {
public:
    MarqueeConsole();
    ~MarqueeConsole();
    void onEnabled() override;
    void display() override;
    void process() override;

private:
    const int REFRESH_DELAY = 120;
    const int POLLING_DELAY = 10;
    String currentCommand;
    std::stringstream outputBuffer;
    int xPos, yPos, dx, dy;
};