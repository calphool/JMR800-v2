/**
 * @file RunModeScreen.cpp
 * @brief Implements the screen for displaying and managing widgets during normal controller operation.
 *
 * The RunModeScreen class is responsible for managing a collection of widgets that are rendered
 * and updated when the controller is in "run" mode. It handles widget drawing, input forwarding,
 * and dynamic screen layout.
 */

#include "screens/RunModeScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/StarFieldWidget.h"

/**
 * @brief Constructs a RunModeScreen instance and initializes internal state.
 */
RunModeScreen::RunModeScreen() {
  log(LOG_VERBOSE, "Inside RunModeScreen->constructor", __func__);
}

/**
 * @brief Destructor for RunModeScreen, which cleans up dynamically allocated widgets.
 */
RunModeScreen::~RunModeScreen() {
  log(LOG_VERBOSE, "Inside RunModeScreen->destructor", __func__);
}


/**
 * @brief Adds a new widget to the screen for coordinated management.
 *
 * @param w Pointer to a dynamically allocated Widget to register with this screen.
 */
void RunModeScreen::addWidget(Widget* w) {
  log(LOG_VERBOSE, "Inside RunModeScreen->addWidget()", __func__);
  widgets.push_back(w);
}


/**
 * @brief Draws the screen contents, including all registered widgets.
 *
 * If the system has been running for fewer than 10 seconds, displays a boot screen.
 * Otherwise, delegates rendering to each widget in order.
 */
void RunModeScreen::draw() {
  log(LOG_VERBOSE, "Inside RunModeScreen->draw()", __func__);
  long m = millis();

   if(m < 10000) {
        ScreenManager::getDisplay()->setCursor(10,0);
        ScreenManager::getDisplay()->print("JMR-800 Controller");
        ScreenManager::getDisplay()->setCursor(10,20);
        ScreenManager::getDisplay()->print(__DATE__); 
        ScreenManager::getDisplay()->setCursor(10,30);
        ScreenManager::getDisplay()->print(__TIME__);;
        ScreenManager::getDisplay()->setCursor(10,50);
        ScreenManager::getDisplay()->print("Booting...");
        if(m % 4 == 0) ScreenManager::getDisplay()->print("|");     
        if(m % 4 == 1) ScreenManager::getDisplay()->print("/");
        if(m % 4 == 2) ScreenManager::getDisplay()->print("-");
        if(m % 4 == 3) ScreenManager::getDisplay()->print("\\");

        ScreenManager::getDisplay()->display();
        return;
  }

  for (Widget* w : widgets) {
        w->draw();
  }
}


/**
 * @brief Forwards input handling to all registered widgets.
 *
 * Called repeatedly in the main loop to process UI interaction.
 */
void RunModeScreen::handleInput() {
  log(LOG_VERBOSE, "Inside RunModeScreen->handleInput()", __func__);

  for (Widget* w : widgets) {
     w->handleInput();
  }
}


/**
 * @brief Called when this screen becomes the active screen.
 *
 * Sets up a minimal layout, including a screen title label and a decorative rectangle.
 */
void RunModeScreen::onEnter() {
  log(LOG_VERBOSE, "Inside RunModeScreen->onEnter()", __func__);
    const char* labelText = "Run Mode";

   Widget* w = new StarFieldWidget(0,0,SCREEN_WIDTH,64);
   addWidget(w);

   Widget* modeLabel = new TextLabelWidget(labelText, (SCREEN_WIDTH - (strlen(labelText) * 6)) / 2, 0, 1, false);
   addWidget(modeLabel);
}


/**
 * @brief Called when the screen is exited or replaced.
 *
 * Responsible for deallocating all dynamic widgets and clearing the widget list.
 */
void RunModeScreen::onExit() {
  log(LOG_VERBOSE, "Inside RunModeScreen->onExit()", __func__);

  for (Widget* w : widgets) {
       delete w;
  }
  widgets.clear();
}
