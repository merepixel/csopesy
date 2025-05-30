#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <unistd.h> 

struct Process {
    int gpu;
    std::string gi;
    std::string ci;
    int pid;
    std::string type;
    std::string processName;
    std::string gpuMemoryUsage;
};

void setConsoleColor(int color) {
    switch (color) {
        case 10: std::cout << "\033[32m"; break; // Green
        case 11: std::cout << "\033[36m"; break; // Cyan
        case 14: std::cout << "\033[33m"; break; // Yellow
        case 15: std::cout << "\033[37m"; break; // White
        case 7:  std::cout << "\033[0m";  break; // Reset
        default: std::cout << "\033[0m"; break;
    }
}

void setCursorPosition(int x, int y) {
    std::cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
}

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

std::string truncateProcessName(const std::string& processName, size_t maxLength) {
    if (processName.length() > maxLength) {
        return "..." + processName.substr(processName.length() - maxLength, maxLength);
    }
    return processName;
}

void displayCurrentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%a %b %d %H:%M:%S %Y", localTime);

    setConsoleColor(11);
    setCursorPosition(0, 1);
    std::cout << buffer << std::endl;
}

void displayGpuSummary() {
    setConsoleColor(11);
    std::cout << "+------------------------------------------------------------------------------------------+" << std::endl;
    std::cout << "| NVIDIA-SMI 551.86                 Driver Version: 551.86          CUDA Version: 12.4     |" << std::endl;
    std::cout << "+-----------------------------------------+-------------------------+----------------------+" << std::endl;
    std::cout << "| GPU  Name                     TCC/WDDM  | Bus-Id           Disp.A | Volatile Uncorr. ECC |" << std::endl;
    std::cout << "| Fan  Temp   Perf          Pwr:Usage/Cap |            Memory-Usage | GPU-Util  Compute M. |" << std::endl;
    std::cout << "|                                         |                         |               MIG M. |" << std::endl;
    std::cout << "|=========================================+=========================+======================+" << std::endl;
    setConsoleColor(14);
    std::cout << "|   0  NVIDIA GeForce GTX 1080     WDDM   |    00000000:26:00.0  On |                  N/A |" << std::endl;
    std::cout << "| 28%   37C    P8             11W / 180W  |     701MiB /    8192MiB |      0%      Default |" << std::endl;
    std::cout << "|                                                                                      N/A |" << std::endl;
    setConsoleColor(11);
    std::cout << "+-----------------------------------------+-------------------------+----------------------+" << std::endl;
    std::cout << "" << std::endl;
}

void displayProcesses(const std::vector<Process>& processes) {
    setConsoleColor(10);
    std::cout << "+------------------------------------------------------------------------------------------+" << std::endl;
    std::cout << "| Processes:                                                                               |" << std::endl;
    std::cout << "|  GPU   GI   CI        PID   Type   Process name                               GPU Memory |" << std::endl;
    std::cout << "|        ID   ID                                                                Usage      |" << std::endl;
    std::cout << "|=========================================+=========================+======================+" << std::endl;

    setConsoleColor(15);
    for (const auto& process : processes) {
        std::string truncatedName = truncateProcessName(process.processName, 36);
        std::cout << "|    0   " << process.gi << "  " << process.ci
                  << "      " << std::setw(4) << process.pid
                  << "    " << process.type
                  << "   " << std::left << std::setw(45) << truncatedName
                  << process.gpuMemoryUsage << "      |" << std::endl;
    }

    setConsoleColor(10);
    std::cout << "+-----------------------------------------+-------------------------+----------------------+" << std::endl;
}

int main() {
    std::vector<Process> processes = {
        {0, "N/A", "N/A", 1368, "C+G", "/System/Library/CoreServices/Finder.app/Contents/MacOS/Finder", "N/A"},
        {0, "N/A", "N/A", 2116, "C+G", "/System/Library/PrivateFrameworks/AppKit.framework/Versions/C/XPCServices/com.apple.ViewBridgeAuxiliary", "N/A"},
        {0, "N/A", "N/A", 4224, "C+G", "/System/Applications/Safari.app/Contents/MacOS/Safari", "N/A"},
        {0, "N/A", "N/A", 5684, "C+G", "/System/Library/CoreServices/SystemUIServer.app/Contents/MacOS/SystemUIServer", "N/A"},
        {0, "N/A", "N/A", 6676, "C+G", "/usrpixel/libexec/WindowServer", "N/A"}
    };

    clearScreen();

    std::string command;
    while (true) {
        std::cout << "Enter command ('nvidia-smi' to display info or 'exit' to quit): ";
        std::getline(std::cin, command);

        if (command == "nvidia-smi") {
            displayCurrentDateTime();
            setCursorPosition(0, 2);
            displayGpuSummary();
            setCursorPosition(0, 14);
            displayProcesses(processes);
            setConsoleColor(7); // Reset
        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Invalid command. Please try again." << std::endl;
        }
    }

    return 0;
}