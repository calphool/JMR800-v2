#pragma once

#include <vector>
#include "modes/AppMode.h"

class ModeManager {
private:
    std::vector<AppMode*> modes;
    int currentIndex = 0;
    bool prevButtonState = false;
    char buffer_64[64];

public:
    void addMode(AppMode* mode);
    void init();                    // Initializes the first mode
    void loop();                    // Handles button logic and delegates
    AppMode* currentMode() const;
};

