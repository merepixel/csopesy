
#include "MarqueeConsole.h"
#include "ConsoleManager.h"
#include "Utils.h"
#include <iostream>
#include <chrono>
#include <thread>

const std::string marqueeText = "This is a Marquee Console";

MarqueeConsole::MarqueeConsole() : AConsole("MARQUEE_CONSOLE"), xPos(0), yPos(4), dx(1), dy(1) {}
MarqueeConsole::~MarqueeConsole() {}

void MarqueeConsole::onEnabled() {
    std::cout << "\033[2J\033[H";
}

void MarqueeConsole::display() {
    ConsoleManager::getInstance()->setCursorPosition(0, 0);
    std::cout << "*** Marquee Console ***\n";
    ConsoleManager::getInstance()->setCursorPosition(xPos, yPos);
    std::cout << marqueeText << std::flush;
    ConsoleManager::getInstance()->setCursorPosition(0, 20);
    std::cout << "Command:\\> " << currentCommand << std::flush;
}


void MarqueeConsole::process() {
    auto start = std::chrono::steady_clock::now();

    while (true) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

        if (elapsed >= POLLING_DELAY) {
            start = now;

            if (kbhit()) {
                char ch = getch();

                if (ch == '\b' && !currentCommand.empty()) {
                    currentCommand.pop_back();
                } else if (ch == '\n' || ch == '\r') {
                    if (currentCommand == "exit") {
                        ConsoleManager::getInstance()->returnToPreviousConsole();
                        return;
                    }
                    currentCommand.clear();
                } else {
                    currentCommand.push_back(ch);
                }
            }
        }

        int termWidth = getTerminalWidth();
        const int fixedMaxY = 18;
        const int minY = 4;

        int maxXPos = std::max(0, termWidth - static_cast<int>(marqueeText.length()));
        if (xPos > maxXPos) xPos = maxXPos;

        xPos += dx;
        yPos += dy;

        if (xPos <= 0 || xPos >= maxXPos) dx = -dx;
        if (yPos <= minY || yPos >= fixedMaxY) dy = -dy;

        std::cout << "\033[2J\033[H";
        display();

        std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_DELAY));
    }
}