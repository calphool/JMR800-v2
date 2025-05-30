#include "screens/ConfigScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "screens/SharedControlScreenUtility.h"

/* --------------------------------------------------------------
   |  Constructor -- initializes internal state of the run screen |
   -------------------------------------------------------------- */
ConfigScreen::ConfigScreen() {
    log(LOG_VERBOSE, "Inside ConfigScreen->constructor");
}


/* --------------------------------------------------------------
   |  Destructor -- releases all dynamically allocated widgets    |
   -------------------------------------------------------------- */
ConfigScreen::~ConfigScreen() {
    log(LOG_VERBOSE, "Inside ConfigScreen->destructor");
}


/* --------------------------------------------------------------
   |  addWidget -- registers a new widget with this screen for   |
   |  coordinated input and draw operations                      |
   -------------------------------------------------------------- */
void ConfigScreen::addWidget(Widget* w) {
  log(LOG_VERBOSE, "Inside ConfigScreen->addWidget()");
  widgets.push_back(w);
}


/* --------------------------------------------------------------
   |  draw -- renders each registered widget in order onto the   |
   |  current display buffer                                     |
   -------------------------------------------------------------- */
void ConfigScreen::draw() {
  log(LOG_VERBOSE, "Inside ConfigScreen->draw()");

  for (Widget* w : widgets) {
        w->draw();
  }
}


/* --------------------------------------------------------------
   |  handleInput -- passes control to each widget to handle     |
   |  input events such as knob movement or encoder clicks       |
   -------------------------------------------------------------- */
void ConfigScreen::handleInput() {
  log(LOG_VERBOSE, "Inside ConfigScreen->handleInput()");

  for (Widget* w : widgets) {
     w->handleInput();
  }
}


/* --------------------------------------------------------------
   |  onEnter -- called when the screen becomes active; used to  |
   |  reset or refresh dynamic elements                          |
   -------------------------------------------------------------- */
void ConfigScreen::onEnter() {
    // Optionally refresh data or reset states
  log(LOG_VERBOSE, "Inside ConfigScreen->onEnter()");
  const char* labelText = "Config Mode";

   TextLabelWidget* dummyBox = nullptr;
   SharedControlScreenUtility::initializeControllerScreenLayout(widgets, labelText, dummyBox);
}


/* --------------------------------------------------------------
   |  onExit -- called when the screen is replaced or hidden;    |
   |  cleanup or persist transient state here                    |
   -------------------------------------------------------------- */
void ConfigScreen::onExit() {
  log(LOG_VERBOSE, "Inside ConfigScreen->onExit()");
  for (Widget* w : widgets) {
      delete w;
  }
  widgets.clear();
}

void ConfigScreen::setActiveKnob(int knobix) {
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