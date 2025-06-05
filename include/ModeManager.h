/**
 * @file ModeManager.h
 * @brief Declares the ModeManager class for managing application modes in JMR800-V2.
 *
 * The ModeManager tracks and switches between multiple application modes,
 * each represented by an AppMode instance. It responds to button input to
 * cycle through modes and delegates control to the currently active mode.
 */

#pragma once

#include <vector>
#include "modes/AppMode.h"

/**
 * @class ModeManager
 * @brief Coordinates transitions between different application modes.
 *
 * The ModeManager maintains a list of AppMode instances, tracks the currently
 * active one, and handles button input for cycling between modes. It is designed
 * to delegate control logic to each mode through a common interface.
 */
class ModeManager {
private:

    /**
     * @brief Container for all registered application modes.
     */
    std::vector<AppMode*> modes;

    /**
     * @brief Index of the currently active mode in the `modes` vector.
     */
    int currentIndex = 0;

    /**
     * @brief Stores the previous button state to detect rising edges.
     */
    bool prevButtonState = false;

    /**
     * @brief A temporary buffer used internally (e.g., for screen rendering or label text).
     */
    char buffer_64[64];

public:

    /**
     * @brief Registers a new application mode.
     * @param mode Pointer to the AppMode instance to add.
     */
    void addMode(AppMode* mode);

    /**
     * @brief Initializes the mode manager and the first active mode.
     *
     * This should be called once during application setup.
     */
    void init();                    // Initializes the first mode

    /**
     * @brief Handles button state changes and delegates control to the current mode.
     *
     * This method should be called regularly in the main loop.
     */
    void loop();                    // Handles button logic and delegates

    /**
     * @brief Returns the currently active application mode.
     * @return Pointer to the current AppMode.
     */
    AppMode* currentMode() const;
};


/**
 * @brief Global instance of the ModeManager.
 *
 * This object is defined in the source file and provides centralized mode control.
 */
extern ModeManager modeManager;