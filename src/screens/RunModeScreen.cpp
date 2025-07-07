/**
 * @file RunModeScreen.cpp
 * @brief Implements the screen for displaying and managing widgets during normal controller operation.
 *
 * The RunModeScreen class is responsible for managing a collection of widgets that are rendered
 * and updated when the controller is in "run" mode. It handles widget drawing, input forwarding,
 * and dynamic screen layout.
 */

#include <cstring>
#include <cstdio>
#include "screens/RunModeScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/StarFieldWidget.h"
#include "IHardwareManager.h"
#include "widgets/CenterWidget.h"
#include <memory>
#include "screens/RunModeWidgets.h"

extern IHardwareManager* hardware;

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
  for (Widget* w : widgets) {
      if(w) {
        delete w;
        w = nullptr;
      }
  }
  widgets.clear();
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
  static uint8_t currentTypecode_ = 0xFF; 
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


  blinkOn_ = !blinkOn_;
  uint knobix = hardware->getLastTouchedKnob();
  if(knobix < NUM_KNOBS) {
    if (hardware->getKnobConfiguration(knobix).typecode != currentTypecode_) {
      currentTypecode_ = hardware->getKnobConfiguration(knobix).typecode;
      runModeWidget =  makeWidget(currentTypecode_);
       for (Widget* w : widgets) {
          if(w->getType() == WidgetType::TextLabel) {
            TextLabelWidget* tlw = static_cast<TextLabelWidget*>(w);
            tlw->setText(" "); // get rid of "RUN MODE" label once a knob has been touched
          }
       }
    }
  }
  
  if(runModeWidget) {
    runModeWidget->draw(hardware->getKnobConfiguration(knobix), hardware->getKnobValue(knobix), blinkOn_);
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

   //knobValueLabel = new TextLabelWidget("", 0, 24, 1, false);
   //addWidget(knobValueLabel);
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
