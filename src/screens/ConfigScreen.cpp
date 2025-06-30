/**
 * @file ConfigScreen.cpp
 * @brief Implements the configuration screen for knob mapping and diagnostics.
 *
 * This screen supports a modal dialog for per-knob configuration and uses the shared control layout.
 */

#include "screens/ConfigScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "screens/SharedControlScreenUtility.h"
#include "screens/KnobConfigDialog.h"
#include "IHardwareManager.h"

extern IHardwareManager* hardware;


/**
 * @brief Constructor for ConfigScreen.
 *
 * Initializes the configuration screen's internal state.
 */
ConfigScreen::ConfigScreen() {
    log(LOG_VERBOSE, "Inside ConfigScreen->constructor", __func__);
}


/**
 * @brief Destructor for ConfigScreen.
 *
 * Cleans up all dynamically allocated widgets and dialogs.
 */
ConfigScreen::~ConfigScreen() {
    log(LOG_VERBOSE, "Inside ConfigScreen->destructor", __func__);
}


/**
 * @brief Adds a widget to the screen’s list for coordinated rendering and input handling.
 * @param w Pointer to a dynamically allocated Widget.
 */
void ConfigScreen::addWidget(Widget* w) {
  log(LOG_VERBOSE, "Inside ConfigScreen->addWidget()", __func__);
  widgets.push_back(w);
}


/**
 * @brief Renders all widgets or the modal knob configuration dialog if active.
 */
void ConfigScreen::draw() {
  log(LOG_VERBOSE, "Inside ConfigScreen->draw()", __func__);

  for (Widget* w : widgets) {
        w->draw();
  }

  if (knobConfigDialog && !knobConfigDialog->isDone()) {
         knobConfigDialog->draw();
   }
}


/**
 * @brief Passes control to either the modal dialog or each registered widget to handle input.
 */
void ConfigScreen::handleInput() {
//  log(LOG_INFO, "Inside ConfigScreen->handleInput()", __func__);

   if (knobConfigDialog) {
//        log(LOG_INFO, "Invoking knobConfigDialog->handleInput()", __func__);

         knobConfigDialog->handleInput();
//         log(LOG_INFO, "Invoking knobConfigDialog->isDone()", __func__);         
         if (knobConfigDialog->isDone()) {
//            log(LOG_INFO, "Invoking deactiveKnobConfig()", __func__);  
            deactiveKnobConfig();
            delay(500);
         }
         return; // If dialog is active, skip other widgets
   }
//  log(LOG_INFO, "invoking widget()->handleInput()", __func__);
  for (Widget* w : widgets) {
     w->handleInput();
  }
}


/**
 * @brief Called when this screen becomes the active screen.
 *
 * Populates the widget list using the shared layout utility.
 */
void ConfigScreen::onEnter() {
  log(LOG_VERBOSE, "Inside ConfigScreen->onEnter()", __func__);
  const char* labelText = "Config Mode";

   TextLabelWidget* dummyBox = nullptr;
   SharedControlScreenUtility::initializeControllerScreenLayout(widgets, labelText, dummyBox);
}


/**
 * @brief Called when the screen is hidden or replaced.
 *
 * Cleans up modal dialogs and all dynamically allocated widgets.
 */
void ConfigScreen::onExit() {
  log(LOG_VERBOSE, "Inside ConfigScreen->onExit()", __func__);

   if (knobConfigDialog) {
         knobConfigDialog->onExit();
         delete knobConfigDialog;
         knobConfigDialog = nullptr;
   }

  for (Widget* w : widgets) {
      delete w;
  }
  widgets.clear();
}

/**
 * @brief Highlights a single potentiometer knob by ID and unhighlights all others.
 * @param knobix Index of the knob to highlight.
 */
void ConfigScreen::highlightActiveKnob(int knobix) {
   for(Widget* w : widgets) {
      if(w->getType() == WidgetType::PotentiometerKnob) {
         PotentiometerKnobWidget* pkw = (PotentiometerKnobWidget*)w;
         if(pkw->getKnobId() == knobix) 
            pkw->setHighlighted(true);
         else
            pkw->setHighlighted(false);
      }
   }
}

/**
 * @brief Initiates modal dialog for configuring the selected knob.
 * @param knobid Index of the knob to configure.
 */
void ConfigScreen::activateKnobConfig(uint knobid) {
   if (knobConfigDialog) return; // already active

   active_knob = knobid;
   knobConfigDialog = new KnobConfigDialog(7, 4, 112, 58, knobid); // Adjust dimensions as needed
   knobConfigDialog->setOnExitCallback([this]() {
   });
   knobConfigDialog->onEnter();
}


void ConfigScreen::deactiveKnobConfig() {
   if (!knobConfigDialog) return;
   
    knobConfigDialog->onExit();
    delete knobConfigDialog;        // no dialog methods on the stack now
    knobConfigDialog = nullptr;

    active_knob = 0;                // reset global selection
    hardware->resetEncoder(0);
    hardware->clearEncoderButton();
    delay(500);
}