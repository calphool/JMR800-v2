#include "screens/ControlsTestScreen.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/PotentiometerKnobWidget.h"
#include "widgets/EncoderKnobWidget.h"
#include "widgets/ButtonWidget.h"
#include "HardwareManager.h"

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

    int textWidth = strlen(labelText) * 6;
    int x = (SCREEN_WIDTH - textWidth) / 2;
    Widget* rectangle = new RectangleWidget(0, 9, SCREEN_WIDTH, 54, false, RectColor::WHITE);
    addWidget(rectangle);
    Widget* modeLabel = new TextLabelWidget(labelText, x, 0, 1, false);
    addWidget(modeLabel);
    numBox = new TextLabelWidget("---", 90, 17, 1, false, LabelColor::WHITE);
    addWidget(numBox);
   
    int iKnobID = 0;
    for(int i=7; i<78;i=i+10) {
      for(int j=14; j<62;j = j + 8) {
         Widget* knob = new PotentiometerKnobWidget(iKnobID, i, j);
         iKnobID++;
         addWidget(knob);
      }
    }

    for(int i=87; i < 98; i = i + 10) {                               // knobs on the right side
      for(int j=30; j < 62; j = j + 8) {
         Widget* knob = new PotentiometerKnobWidget(iKnobID, i, j);
         iKnobID++;
         addWidget(knob);
      }
    }

    Widget* encoderKnob = new EncoderKnobWidget(109,18);
    addWidget(encoderKnob);

    Widget* miniScreen = new RectangleWidget(88, 15, 21, 11, false, RectColor::WHITE);
    addWidget(miniScreen);

    int iButtonCtr = 0;
    for(int j=30; j < 62; j = j + 8) {  
      Widget* button = new ButtonWidget(iButtonCtr, 109, j);
      iButtonCtr++;
      addWidget(button);
    }
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
 
