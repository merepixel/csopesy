#include "ConsoleManager.h"
#include <iostream>
#include "MainConsole.h"
#include "MarqueeConsole.h"
#include "Utils.h"

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance() {
    return sharedInstance;
}

void ConsoleManager::initialize() {
    sharedInstance = new ConsoleManager();
}

void ConsoleManager::destroy() {
    delete sharedInstance;
}

ConsoleManager::ConsoleManager() {
    this->running = true;
    this->consoleTable[MAIN_CONSOLE] = std::make_shared<MainConsole>();
    this->consoleTable[MARQUEE_CONSOLE] = std::make_shared<MarqueeConsole>();
    this->switchConsole(MAIN_CONSOLE);
}

ConsoleManager::~ConsoleManager() {}

void ConsoleManager::drawConsole() const {
    if (currentConsole) currentConsole->display();
}

void ConsoleManager::process() const {
    if (currentConsole) currentConsole->process();
}

void ConsoleManager::switchConsole(String consoleName) {
    if (consoleTable.count(consoleName)) {
        std::cout << "\033[2J\033[H";
        previousConsole = currentConsole;
        currentConsole = consoleTable.at(consoleName);
        currentConsole->onEnabled();
    }
}

void ConsoleManager::returnToPreviousConsole() {
    if (previousConsole) {
        currentConsole = previousConsole;
        previousConsole = nullptr;
        std::cout << "\033[2J\033[H";
        currentConsole->display();
    }
}

bool ConsoleManager::isRunning() const {
    return running;
}

std::shared_ptr<AConsole> ConsoleManager::getCurrentConsole() {
    return currentConsole;
}

bool ConsoleManager::isScreenRegistered(const String& screenName) {
    return consoleTable.find(screenName) != consoleTable.end();
}

void ConsoleManager::setCursorPosition(int x, int y) const {
    std::cout << "\033[" << y + 1 << ";" << x + 1 << "H";
}