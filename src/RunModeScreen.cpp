#include "RunModeScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "RectangleWidget.h"
#include "TextLabelWidget.h"

/* --------------------------------------------------------------
   |  Constructor -- initializes internal state of the run screen |
   -------------------------------------------------------------- */
RunModeScreen::RunModeScreen() {
  log(LOG_VERBOSE, "Inside RunModeScreen->constructor");
}

/* --------------------------------------------------------------
   |  Destructor -- releases all dynamically allocated widgets    |
   -------------------------------------------------------------- */
RunModeScreen::~RunModeScreen() {
  log(LOG_VERBOSE, "Inside RunModeScreen->destructor");
}


/* --------------------------------------------------------------
   |  addWidget -- registers a new widget with this screen for   |
   |  coordinated input and draw operations                      |
   -------------------------------------------------------------- */
void RunModeScreen::addWidget(Widget* w) {
  log(LOG_VERBOSE, "Inside RunModeScreen->addWidget()");
  widgets.push_back(w);
}


/* --------------------------------------------------------------
   |  draw -- renders each registered widget in order onto the   |
   |  current display buffer                                     |
   -------------------------------------------------------------- */
void RunModeScreen::draw() {
  log(LOG_VERBOSE, "Inside RunModeScreen->draw()");

  for (Widget* w : widgets) {
        w->draw();
  }
}


/* --------------------------------------------------------------
   |  handleInput -- passes control to each widget to handle     |
   |  input events such as knob movement or encoder clicks       |
   -------------------------------------------------------------- */
void RunModeScreen::handleInput() {
  log(LOG_VERBOSE, "Inside RunModeScreen->handleInput()");

  for (Widget* w : widgets) {
     w->handleInput();
  }
}


/* --------------------------------------------------------------
   |  onEnter -- called when the screen becomes active; used to  |
   |  reset or refresh dynamic elements                          |
   -------------------------------------------------------------- */
void RunModeScreen::onEnter() {
  log(LOG_VERBOSE, "Inside RunModeScreen->onEnter()");
    const char* labelText = "RUN MODE";

    // Estimate width for centering using default font size 1 (6px/char approx)
    int textWidth = strlen(labelText) * 6;
    int x = (SCREEN_WIDTH - textWidth) / 2;
    int y = 2;
    Widget* rectangle = new RectangleWidget(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false, RectColor::WHITE);
    addWidget(rectangle);
    Widget* modeLabel = new TextLabelWidget(ScreenManager::getDisplay(), labelText, x, y, 1, false);
    addWidget(modeLabel);
}


/* --------------------------------------------------------------
   |  onExit -- called when the screen is replaced or hidden;    |
   |  cleanup or persist transient state here                    |
   -------------------------------------------------------------- */
void RunModeScreen::onExit() {
  log(LOG_VERBOSE, "Inside RunModeScreen->onExit()");

  for (Widget* w : widgets) {
       delete w;
  }
  widgets.clear();
}
