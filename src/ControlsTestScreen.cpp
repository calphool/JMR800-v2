#include "ControlsTestScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "RectangleWidget.h"
#include "TextLabelWidget.h"

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
}


/* --------------------------------------------------------------
   |  onEnter -- called when the screen becomes active; used to  |
   |  reset or refresh dynamic elements                          |
   -------------------------------------------------------------- */
void ControlsTestScreen::onEnter() {
    log(LOG_VERBOSE, "Inside ControlsTestScreen->onEnter()");
    const char* labelText = "Controls Test Mode";

    // Estimate width for centering using default font size 1 (6px/char approx)
    int textWidth = strlen(labelText) * 6;
    int x = (SCREEN_WIDTH - textWidth) / 2;
    int y = 2;
    Widget* rectangle = new RectangleWidget(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false, RectColor::WHITE);
    addWidget(rectangle);
    Widget* modeLabel = new TextLabelWidget(ScreenManager::getDisplay(), labelText, x, y, 1, false);
    addWidget(modeLabel);

    //TODO: add code to create all the knob and button widgets, and put them in an array
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
 
