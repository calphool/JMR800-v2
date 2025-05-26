#include "DirectCommandScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "RectangleWidget.h"
#include "TextLabelWidget.h"

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
  const char* labelText = "DIRECT COMMAND MODE";

  // Estimate width for centering using default font size 1 (6px/char approx)
  int textWidth = strlen(labelText) * 6;
  int x = (SCREEN_WIDTH - textWidth) / 2;
  int y = 2;
  Widget* rectangle = new RectangleWidget(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, false, RectColor::WHITE);
  addWidget(rectangle);
  Widget* modeLabel = new TextLabelWidget(ScreenManager::getDisplay(), labelText, x, y, 1, false);
  addWidget(modeLabel);

  //TODO: add code to create the two input text controls and buttons: command byte, data byte, send button 

}


/* --------------------------------------------------------------
   |  onExit -- called when the screen is replaced or hidden;    |
   |  cleanup or persist transient state here                    |
   -------------------------------------------------------------- */
void DirectCommandScreen::onExit() {
  log(LOG_VERBOSE, "Inside DirectCommandScreen->onExit()");
    log(LOG_VERBOSE, "Inside DirectCommandScreen->destructor");
    for (Widget* w : widgets) {
        delete w;
    }
    widgets.clear();
}
 
 
