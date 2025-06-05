/**
 * @file ScreenManager.h
 * @brief Declares the ScreenManager class responsible for UI screen control on the OLED display.
 *
 * The ScreenManager handles the active UI screen logic, regularly invokes its loop method,
 * and manages access to the shared Adafruit_SH1106G OLED display used throughout the interface.
 *
 * This is part of the JMR800-V2 project running on a Teensy 4.1 microcontroller.
 */

#pragma once

#include <Adafruit_SH110X.h>
#include "screens/Screen.h"


/**
 * @class ScreenManager
 * @brief Controls the active UI screen and manages the OLED display lifecycle.
 *
 * The ScreenManager holds a pointer to the currently active screen and periodically calls
 * its loop method. It also initializes and exposes the shared Adafruit_SH1106G display
 * instance used by all screens.
 */
class ScreenManager {
private:
    /**
     * @brief Pointer to the currently active screen.
     */
    Screen* activeScreen = nullptr;

    /**
     * @brief Timestamp (in milliseconds) of the last screen loop update.
     */
    unsigned long lastScreenLoop = 0;

    /**
     * @brief Static instance of the SH1106 OLED display used across all screens.
     */
    static Adafruit_SH1106G display;

public:
    /**
     * @brief Initializes the display and prepares the screen manager for use.
     *
     * Should be called once during system setup.
     */
    void init();

    /**
     * @brief Sets the active screen to be rendered and updated.
     * @param screen Pointer to a new screen object that will become active.
     */
    void setScreen(Screen* screen);

    /**
     * @brief Updates the current screen if enough time has passed since the last update.
     *
     * This method should be called from the main loop to ensure timely UI updates.
     */
    void loop();

    /**
     * @brief Retrieves the currently active screen.
     * @return Pointer to the current active screen.
     */
    Screen* getScreen() const;

    /**
     * @brief Provides access to the static OLED display instance.
     * @return Pointer to the Adafruit_SH1106G display.
     */
    static Adafruit_SH1106G* getDisplay();
};

/**
 * @brief Global instance of the ScreenManager declared for use throughout the system.
 *
 * Defined in a source file (typically ScreenManager.cpp), this instance is used
 * to manage the UI from application code.
 */
extern ScreenManager screenManager;

/**
 * @brief Millisecond interval between screen loop executions.
 *
 * Used to throttle how frequently the `loop()` method executes.
 */
constexpr unsigned long ScreenManagerLoopMS = 33;
