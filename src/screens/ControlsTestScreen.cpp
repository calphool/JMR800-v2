#include "screens/ControlsTestScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/PotentiometerKnobWidget.h"
#include "widgets/EncoderKnobWidget.h"
#include "widgets/ButtonWidget.h"
#include "HardwareManager.h"
#include "screens/SharedControlScreenUtility.h"

/* --------------------------------------------------------------
   |  Constructor -- initializes internal state of the run screen |
   -------------------------------------------------------------- */
ControlsTestScreen::ControlsTestScreen() {
      log(LOG_VERBOSE, "Inside ControlsTestScreen->constructor");
}

/* --------------------------------------------------------------
   |  Destructor -- releases all dynamically allocated widgets    |
   -------------------------------------------------------------- */
ControlsTestScreen::~ControlsTestScreen() {
      log(LOG_VERBOSE, "Inside ControlsTestScreen->destructor");
}


/* --------------------------------------------------------------
   |  addWidget -- registers a new widget with this screen for   |
   |  coordinated input and draw operations                      |
   -------------------------------------------------------------- */
void ControlsTestScreen::addWidget(Widget* w) {
  log(LOG_VERBOSE, "Inside ControlsTestScreen->addWidget()");
  widgets.push_back(w);
}


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


/* --------------------------------------------------------------
   |  draw -- renders each registered widget in order onto the   |
   |  current display buffer                                     |
   -------------------------------------------------------------- */
void ControlsTestScreen::draw() {
  log(LOG_VERBOSE, "Inside ControlsTestScreen->draw()");

  for (Widget* w : widgets) {
        w->draw();
  }
}



/* --------------------------------------------------------------
   |  handleInput -- passes control to each widget to handle     |
   |  input events such as knob movement or encoder clicks       |
   -------------------------------------------------------------- */
void ControlsTestScreen::handleInput() {
  log(LOG_VERBOSE, "Inside ControlsTestScreen->handleInput()");

  for (Widget* w : widgets) {
     w->handleInput();
  }

  char buf[4];
  truncateTo3Right(hardware.getEncoderValue(), buf);
  numBox->setText(buf);
}


/* --------------------------------------------------------------
   |  onEnter -- called when the screen becomes active; used to  |
   |  reset or refresh dynamic elements                          |
   -------------------------------------------------------------- */
void ControlsTestScreen::onEnter() {
    log(LOG_VERBOSE, "Inside ControlsTestScreen->onEnter()");
    const char* labelText = "Controls Test Mode";

   SharedControlScreenUtility::initializeControllerScreenLayout(widgets, labelText, numBox);
}


/* --------------------------------------------------------------
   |  onExit -- called when the screen is replaced or hidden;    |
   |  cleanup or persist transient state here                    |
   -------------------------------------------------------------- */
void ControlsTestScreen::onExit() {
  log(LOG_VERBOSE, "Inside ControlsTestScreen->onExit()");
  for (Widget* w : widgets) {
      delete w;
  }
  widgets.clear();
}
 
