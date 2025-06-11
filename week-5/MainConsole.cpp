#include "MainConsole.h"
#include "ConsoleManager.h"
#include "Utils.h"
#include <iostream>
#include <sstream>

MainConsole::MainConsole() : AConsole("MAIN_CONSOLE") {}

void MainConsole::displayHeader() {
    printHeader("3D_CSOPESY.txt");
}

void MainConsole::onEnabled() {
    displayHeader();
}

void MainConsole::display() {
    displayHeader();
    process();
}

void MainConsole::process() {
    String command;
    while (ConsoleManager::getInstance()->getCurrentConsole().get() == this) {
        std::cout << "root:\\> ";
        std::getline(std::cin, command);
        handleCommand(command);
    }
}

void MainConsole::handleCommand(String command) {
    command = toLowerCase(command);
    auto manager = ConsoleManager::getInstance();

    if (command == "exit") {
        printMsgNewLine("Exiting...");
        exit(0);
    } else if (command == "clear" || command == "cls") {
        std::cout << "\033[2J\033[H";
        displayHeader();
    } else if (command == "marquee") {
        manager->switchConsole(MARQUEE_CONSOLE);
    } else {
        printMsgNewLine("Unknown command.");
    }
}