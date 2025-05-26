#include "ScreenManager.h"
#include "defines.h"
#include "Logging.h"

ScreenManager screenManager;

// Define the static display instance
Adafruit_SH1106G ScreenManager::display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


/* --------------------------------------------------------------
   |  init -- initializes the OLED display with default settings |
   |  and prepares the screen buffer for rendering UI elements   |
   -------------------------------------------------------------- */
void ScreenManager::init() {
    log(LOG_VERBOSE, "inside ScreenManager->init(), initializing screen");

    display.begin(OLED_I2C_ADDRESS, false);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setTextWrap(false);
    display.display();
}

/* --------------------------------------------------------------
   |  setScreen -- switches the active screen to the given one,  |
   |  calling onExit() on the current screen and onEnter() on    |
   |  the new one                                                |
   -------------------------------------------------------------- */
void ScreenManager::setScreen(Screen* screen) {
  log(LOG_VERBOSE, "inside ScreenManager->setScreen()");

  if (activeScreen) activeScreen->onExit();
  activeScreen = screen;
  if (activeScreen) activeScreen->onEnter();
}


/* --------------------------------------------------------------
   |  loop -- periodically invokes handleInput() and draw() on   |
   |  the active screen and updates the OLED display buffer      |
   -------------------------------------------------------------- */
void ScreenManager::loop() {
    if (!activeScreen) return;
    if (millis() - lastScreenLoop < ScreenManagerLoopMS) return;
    lastScreenLoop = millis();

    activeScreen->handleInput();
    display.clearDisplay();
    activeScreen->draw();
    display.display();
}

/* --------------------------------------------------------------
   |  getScreen -- returns a pointer to the currently active     |
   |  screen object, or nullptr if none is active                |
   -------------------------------------------------------------- */
Screen* ScreenManager::getScreen() const {
  log(LOG_VERBOSE, "inside ScreenManager->getScreen()");
  return activeScreen;
}


/* --------------------------------------------------------------
   |  getDisplay -- returns a pointer to the currently assigned  |
   |  Adafruit_SH1106G display object                            |
   -------------------------------------------------------------- */
Adafruit_SH1106G* ScreenManager::getDisplay()  {
  log(LOG_VERBOSE, "inside ScreenManager->getScreen()");
  return &display;
}
