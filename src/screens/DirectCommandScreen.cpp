/**
 * @file DirectCommandScreen.cpp
 * @brief Implements a screen for sending raw parameter/value commands to the synthesizer.
 *
 * This screen allows the user to manipulate two numeric inputs (command and value) and a push button
 * to transmit them. Encoder-based navigation allows toggling between controls.
 */

#include "screens/DirectCommandScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/EncoderAttachedNumericWidget.h"

/**
 * @brief Constructor for DirectCommandScreen.
 *
 * Initializes the internal widget list and logs screen creation.
 */
DirectCommandScreen::DirectCommandScreen() {
    log(LOG_VERBOSE, "Inside DirectCommandScreen->constructor", __func__);
}

/**
 * @brief Destructor for DirectCommandScreen.
 *
 * Frees all dynamically allocated widgets upon screen teardown.
 */
DirectCommandScreen::~DirectCommandScreen() {
    log(LOG_VERBOSE, "Inside DirectCommandScreen->destructor", __func__);
}


/**
 * @brief Adds a widget to the screen's managed widget list.
 *
 * @param w Pointer to the widget to add.
 */
void DirectCommandScreen::addWidget(Widget* w) {
  log(LOG_VERBOSE, "Inside DirectCommandScreen->addWidget()", __func__);
  widgets.push_back(w);
}


/**
 * @brief Renders all registered widgets to the OLED display.
 */
void DirectCommandScreen::draw() {
  log(LOG_VERBOSE, "Inside DirectCommandScreen->draw()", __func__);

  for (Widget* w : widgets) {
        w->draw();
  }
}


/**
 * @brief Dispatches input handling to all widgets in the screen.
 */
void DirectCommandScreen::handleInput() {
  log(LOG_VERBOSE, "Inside DirectCommandScreen->handleInput()", __func__);

  for (Widget* w : widgets) {
     w->handleInput();
  }
}


/**
 * @brief Called when the screen is activated.
 *
 * Initializes and arranges all UI components and attaches the encoder to the first input.
 */
void DirectCommandScreen::onEnter() {
    // Optionally refresh data or reset states
  log(LOG_VERBOSE, "Inside DirectCommandScreen->onEnter()", __func__);
  const char* labelText = "Direct Command Mode";

  strcpy(buff, "%02X");

   int textWidth = strlen(labelText) * 6;
   int x = (SCREEN_WIDTH - textWidth) / 2;
   Widget* rectangle = new RectangleWidget(0, 9, SCREEN_WIDTH, 54, false, RectColor::WHITE);
   addWidget(rectangle);
   Widget* modeLabel = new TextLabelWidget(labelText, x, 0, 1, false);
   addWidget(modeLabel);

   Widget* cmdLabel = new TextLabelWidget(" CMD:    BYTE:   ", 12, 20);
   addWidget(cmdLabel);

  cmdWidget = new EncoderAttachedNumericWidget(48, 20, 0, 255, buff);
  cmdWidget->attachToEncoder();
  cmdWidget->setHighlighted(true);
  addWidget(cmdWidget);
  byteWidget = new EncoderAttachedNumericWidget(102, 20, 0, 255, buff);
  byteWidget->detachFromEncoder();
  byteWidget->setHighlighted(false);
  addWidget(byteWidget);

  pushWidget = new PushButtonWidget("Send", 50, 40);
  addWidget(pushWidget);
}


/**
 * @brief Advances encoder focus between the command box, byte box, and send button.
 */
void DirectCommandScreen::advanceActiveControl() {
   if(cmdWidget == NULL || byteWidget == NULL || pushWidget == NULL) {
      log(LOG_ERROR, "undefined control in DirectCommandScreen::advanceActiveContr()", __func__);
      return;
   }
   
   if(cmdWidget->isAttachedToEncoder()) {
      cmdWidget->detachFromEncoder();
      pushWidget->detachFromEncoder();
      byteWidget->attachToEncoder();
      cmdWidget->setHighlighted(false);
      pushWidget->setHighlighted(false);
      byteWidget->setHighlighted(true);
   }
   else
   if(byteWidget->isAttachedToEncoder()) {
      cmdWidget->detachFromEncoder();
      pushWidget->attachToEncoder();
      byteWidget->detachFromEncoder();
      cmdWidget->setHighlighted(false);
      pushWidget->setHighlighted(true);
      byteWidget->setHighlighted(false);
   }
   else { // we're on the pushButton and transitioning off of it
      cmdWidget->attachToEncoder();
      pushWidget->detachFromEncoder();
      byteWidget->detachFromEncoder();
      cmdWidget->setHighlighted(true);
      pushWidget->setHighlighted(false);
      byteWidget->setHighlighted(false);
   }
}


/**
 * @brief Retrieves the numeric value from the command input field.
 * @return The current command value (0–255).
 */
uint DirectCommandScreen::getCmdValue() {
   if(cmdWidget)
      return cmdWidget->getValue();
   
   return 0;
}


/**
 * @brief Retrieves the numeric value from the byte input field.
 * @return The current byte value (0–255).
 */
uint DirectCommandScreen::getByteValue() {
   if(cmdWidget)
      return byteWidget->getValue();
   
   return 0;
}


/**
 * @brief Returns a pointer to the currently encoder-attached widget.
 * @return Active widget pointer or nullptr if none attached.
 */
Widget* DirectCommandScreen::getActiveWidget() {
   if(cmdWidget->isAttachedToEncoder())
      return cmdWidget;

   if(pushWidget->isAttachedToEncoder())
      return pushWidget;

   if(byteWidget->isAttachedToEncoder())
      return byteWidget;

   return NULL;
}


/**
 * @brief Called when the screen is deactivated or replaced.
 *
 * Cleans up all dynamic widgets.
 */
void DirectCommandScreen::onExit() {
  log(LOG_VERBOSE, "Inside DirectCommandScreen->onExit()", __func__);
    for (Widget* w : widgets) {
        delete w;
    }
    widgets.clear();
}
