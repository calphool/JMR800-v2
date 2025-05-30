#include "widgets/Widget.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


class PushButtonWidget : public Widget {
public:
    PushButtonWidget(const char* label, int x, int y);

    void setPressed(bool state);
    void draw() override;
    void handleInput();

    void attachToEncoder();
    void detachFromEncoder();
    bool isAttachedToEncoder();


private:
    bool pressed;
    char text[25];
    bool bIsAttachedToEncoder;


protected:
    WidgetType getType() const override;
};


