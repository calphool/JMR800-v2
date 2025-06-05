/**
 * @file AppMode.h
 * @brief Abstract interface for application mode behavior.
 *
 * This interface defines the lifecycle and display logic required for any
 * operational mode in the JMR800-V2 system, including setup, run, and config modes.
 */

#pragma once

#include "screens/Screen.h"

/**
 * @class AppMode
 * @brief Base class for defining behavior of application modes.
 *
 * An application mode corresponds to a distinct UI and operational context,
 * such as configuration, test, or performance modes. Each mode provides a screen,
 * lifecycle callbacks, and a loop handler.
 */
class AppMode {
public:

    /**
     * @brief Returns a human-readable name for the mode.
     * @return Constant string representing the mode name
     */
    virtual const char* name() const = 0;

    /**
     * @brief Returns the screen object associated with this mode.
     * @return Pointer to the active `Screen` instance
     */
    virtual Screen* getScreen() = 0;

    /**
     * @brief Called once when entering the mode.
     */
    virtual void onEnter() = 0;

    /**
     * @brief Called once when exiting the mode.
     */
    virtual void onExit() = 0;

    /**
     * @brief Called periodically while the mode is active.
     */
    virtual void loop() = 0;

    /**
     * @brief Virtual destructor for safe cleanup.
     */
    virtual ~AppMode() {}

    /**
     * @brief Invoked to explicitly trigger screen display or refresh.
     */
    virtual void showScreen() = 0;
};

