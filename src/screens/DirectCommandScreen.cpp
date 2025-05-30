#include "screens/DirectCommandScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/EncoderAttachedNumericWidget.h"

/* --------------------------------------------------------------
   |  Constructor -- initializes internal state of the run screen |
   -------------------------------------------------------------- */
DirectCommandScreen::DirectCommandScreen() {
    log(LOG_VERBOSE, "Inside DirectCommandScreen->constructor");
}

/* --------------------------------------------------------------
   |  Destructor -- releases all dynamically allocated widgets    |
   -------------------------------------------------------------- */
DirectCommandScreen::~DirectCommandScreen() {
    log(LOG_VERBOSE, "Inside DirectCommandScreen->destructor");
}


/* --------------------------------------------------------------
   |  addWidget -- registers a new widget with this screen for   |
   |  coordinated input and draw operations                      |
   -------------------------------------------------------------- */
void DirectCommandScreen::addWidget(Widget* w) {
  log(LOG_VERBOSE, "Inside DirectCommandScreen->addWidget()");
  widgets.push_back(w);
}


/* --------------------------------------------------------------
   |  draw -- renders each registered widget in order onto the   |
   |  current display buffer                                     |
   -------------------------------------------------------------- */
void DirectCommandScreen::draw() {
  log(LOG_VERBOSE, "Inside DirectCommandScreen->draw()");

  for (Widget* w : widgets) {
        w->draw();
  }
}


/* --------------------------------------------------------------
   |  handleInput -- passes control to each widget to handle     |
   |  input events such as knob movement or encoder clicks       |
   -------------------------------------------------------------- */
void DirectCommandScreen::handleInput() {
  log(LOG_VERBOSE, "Inside DirectCommandScreen->handleInput()");

  for (Widget* w : widgets) {
     w->handleInput();
  }
}


/* --------------------------------------------------------------
   |  onEnter -- called when the screen becomes active; used to  |
   |  reset or refresh dynamic elements                          |
   -------------------------------------------------------------- */
void DirectCommandScreen::onEnter() {
    // Optionally refresh data or reset states
  log(LOG_VERBOSE, "Inside DirectCommandScreen->onEnter()");
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

void DirectCommandScreen::advanceActiveControl() {
   if(cmdWidget == NULL || byteWidget == NULL || pushWidget == NULL) {
      log(LOG_ERROR, "undefined control in DirectCommandScreen::advanceActiveContr()");
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

uint DirectCommandScreen::getCmdValue() {
   if(cmdWidget)
      return cmdWidget->getValue();
   
   return 0;
}


uint DirectCommandScreen::getByteValue() {
   if(cmdWidget)
      return byteWidget->getValue();
   
   return 0;
}


Widget* DirectCommandScreen::getActiveWidget() {
   if(cmdWidget->isAttachedToEncoder())
      return cmdWidget;

   if(pushWidget->isAttachedToEncoder())
      return pushWidget;

   if(byteWidget->isAttachedToEncoder())
      return byteWidget;

   return NULL;
}


/* --------------------------------------------------------------
   |  onExit -- called when the screen is replaced or hidden;    |
   |  cleanup or persist transient state here                    |
   -------------------------------------------------------------- */
void DirectCommandScreen::onExit() {
  log(LOG_VERBOSE, "Inside DirectCommandScreen->onExit()");
    for (Widget* w : widgets) {
        delete w;
    }
    widgets.clear();
}
