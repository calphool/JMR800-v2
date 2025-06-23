/**
 * @file ControlsTestScreen.cpp
 * @brief Implements a screen for testing all physical controls: knobs, buttons, and encoder.
 *
 * This screen visually renders the control states and highlights real-time interaction.
 * It uses shared layout utilities for a consistent grid arrangement.
 */

#include <cstdio>
#include <cstring>
#include "screens/ControlsTestScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/PotentiometerKnobWidget.h"
#include "widgets/EncoderKnobWidget.h"
#include "widgets/ButtonWidget.h"
#include "screens/SharedControlScreenUtility.h"
#include "HardwareInterface.h"

extern HardwareInterface* hardware;

/**
 * @brief Constructor for ControlsTestScreen.
 *
 * Initializes the screen and logs its creation.
 */
ControlsTestScreen::ControlsTestScreen() {
      log(LOG_VERBOSE, "Inside ControlsTestScreen->constructor", __func__);
}


/**
 * @brief Destructor for ControlsTestScreen.
 *
 * Cleans up any dynamically allocated widgets.
 */
ControlsTestScreen::~ControlsTestScreen() {
      log(LOG_VERBOSE, "Inside ControlsTestScreen->destructor", __func__);
}


/**
 * @brief Adds a widget to the screen's widget list for rendering and input handling.
 * @param w Pointer to the widget to add.
 */
void ControlsTestScreen::addWidget(Widget* w) {
  log(LOG_VERBOSE, "Inside ControlsTestScreen->addWidget()", __func__);
  widgets.push_back(w);
}


/**
 * @brief Helper function to truncate a long value to its last three digits.
 * @param value The long value to truncate.
 * @param outStr Output string buffer to hold the truncated digits (at least 4 bytes).
 */
void truncateTo3Right(long value, char* outStr) {
    char temp[20];  // enough to hold any 64-bit integer as a string
    snprintf(temp, sizeof(temp), "%ld", value);  // convert to string

    size_t len = strlen(temp);
    if (len > 3) {
        // Copy last 3 characters
        strcpy(outStr, temp + len - 3);
    } else {
        // Copy whole string if length <= 3
        strcpy(outStr, temp);
    }
}


/**
 * @brief Draws all registered widgets onto the display.
 */
void ControlsTestScreen::draw() {
  log(LOG_VERBOSE, "Inside ControlsTestScreen->draw()", __func__);

  for (Widget* w : widgets) {
        w->draw();
  }
}



/**
 * @brief Handles input by delegating to widgets and updating the encoder value display.
 */
void ControlsTestScreen::handleInput() {
  log(LOG_VERBOSE, "Inside ControlsTestScreen->handleInput()", __func__);

  for (Widget* w : widgets) {
     w->handleInput();
  }

  char buf[4];
  truncateTo3Right(hardware->getEncoderValue(), buf);
  numBox->setText(buf);
}


/**
 * @brief Called when the screen is first shown; sets up widget layout and label.
 *
 * Uses a shared layout helper to configure knobs, buttons, and encoder display.
 */
void ControlsTestScreen::onEnter() {
    log(LOG_VERBOSE, "Inside ControlsTestScreen->onEnter()", __func__);
    const char* labelText = "Controls Test Mode";

   SharedControlScreenUtility::initializeControllerScreenLayout(widgets, labelText, numBox);
}


/**
 * @brief Called when the screen is no longer active.
 *
 * Cleans up all dynamically allocated widgets.
 */
void ControlsTestScreen::onExit() {
  log(LOG_VERBOSE, "Inside ControlsTestScreen->onExit()", __func__);
  for (Widget* w : widgets) {
      delete w;
  }
  widgets.clear();
}
 
