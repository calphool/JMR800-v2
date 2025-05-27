#include "widgets/Widget.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


class ButtonWidget : public Widget {
public:
    ButtonWidget(int x, int y);

    void setPressed(bool state);
    void setLEDs(bool bRed, bool bGreen);
    void draw() override;
    void handleInput();

private:
    bool pressed;
    bool red;
    bool green;
};


