#include "widgets/KnobWidget.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


class EncoderKnobWidget : public KnobWidget {
public:
    EncoderKnobWidget(int x, int y);

    void setPressed(bool state);
    void draw() override;
    void handleInput() override;

private:
    bool pressed;

protected:
    WidgetType getType() const override;
};


