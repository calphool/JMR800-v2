/**
 * @file ScreenManager.cpp
 * @brief Implements the ScreenManager class for managing the OLED display and screen lifecycle.
 *
 * Responsible for initializing the SH1106 OLED display, switching between active screen instances,
 * invoking UI lifecycle methods, and coordinating display updates.
 */


#include "ScreenManager.h"
#include "defines.h"
#include "Logging.h"

/// Global singleton instance of the ScreenManager
ScreenManager screenManager;

/// Static display driver instance used for rendering UI elements
Adafruit_SH1106G ScreenManager::display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  // SDA = 18, SCL = 19 for Teensy 4.1


/**
 * @brief Initializes the OLED display with default settings.
 *
 * Sets up the I2C display, clears the screen, sets text size and color, and renders an empty buffer.
 * Should be called once during application setup.
 */
void ScreenManager::init() {
    log(LOG_VERBOSE, "inside ScreenManager->init(), initializing screen");

    display.begin(OLED_I2C_ADDRESS, false);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setTextWrap(false);
    display.display();
}

/**
 * @brief Sets the active screen instance and manages lifecycle transitions.
 *
 * Calls `onExit()` on the current screen and `onEnter()` on the new screen to ensure
 * proper state transitions. This method may be called repeatedly to switch views.
 *
 * @param screen Pointer to the new screen to activate.
 */
void ScreenManager::setScreen(Screen* screen) {
  log(LOG_VERBOSE, "inside ScreenManager->setScreen()");

  if (activeScreen) activeScreen->onExit();
  activeScreen = screen;
  if (activeScreen) activeScreen->onEnter();
}


/**
 * @brief Main loop function that updates and redraws the active screen.
 *
 * This method is invoked periodically. It calls `handleInput()` and `draw()` on the
 * active screen and updates the OLED display buffer.
 */
void ScreenManager::loop() {
    if (!activeScreen) return;
    if (millis() - lastScreenLoop < ScreenManagerLoopMS) return;
    lastScreenLoop = millis();

    activeScreen->handleInput();
    display.clearDisplay();
    activeScreen->draw();
    display.display();
}

/**
 * @brief Retrieves the currently active screen.
 *
 * @return Screen* Pointer to the active screen object, or nullptr if no screen is active.
 */
Screen* ScreenManager::getScreen() const {
  log(LOG_VERBOSE, "inside ScreenManager->getScreen()");
  return activeScreen;
}


/**
 * @brief Accessor for the internal OLED display driver.
 *
 * Provides external code access to the underlying Adafruit_SH1106G display object for
 * low-level drawing or cursor positioning.
 *
 * @return Adafruit_SH1106G* Pointer to the initialized display object.
 */
Adafruit_SH1106G* ScreenManager::getDisplay()  {
  log(LOG_VERBOSE, "inside ScreenManager->getScreen()");
  return &display;
}