#pragma once
#include "TypedefRepo.h"

class AConsole {
public:
    AConsole(String name);
    virtual ~AConsole() = default;

    String getName();
    virtual void onEnabled() = 0;
    virtual void display() = 0;
    virtual void process() = 0;

protected:
    String name;
};