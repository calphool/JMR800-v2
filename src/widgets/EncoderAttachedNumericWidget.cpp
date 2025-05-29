#include "widgets/EncoderAttachedNumericWidget.h"
#include "widgets/Widget.h"
#include "ScreenManager.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "HardwareManager.h"



EncoderAttachedNumericWidget::EncoderAttachedNumericWidget(int x, int y, int min, int max, char* _printFormat) : Widget(x,y, false), 
    min(min), max(max), printFormat(_printFormat)    
{
    control_value = 0;
};

void EncoderAttachedNumericWidget::draw() {
    char buff[20];
    toggle = !toggle;

    ScreenManager::getDisplay()->setCursor(x, y);
    sprintf(buff, printFormat, control_value);
    ScreenManager::getDisplay()->print(buff);

    if(bHighlighted) {
        if(toggle) 
            ScreenManager::getDisplay()->drawLine(x, y + 7, x + strlen(buff) * 6, y + 7, 0);
        else
            ScreenManager::getDisplay()->drawLine(x, y + 7, x + strlen(buff) * 6, y + 7, 1);
    }
}

void EncoderAttachedNumericWidget::handleInput() {
    if(bIsAttachedToEncoder) {
        control_value = hardware.getEncoderZeroTo(max + min + 1) - min;
    }
}

bool EncoderAttachedNumericWidget::isAttachedToEncoder() {
    return bIsAttachedToEncoder;
}

void EncoderAttachedNumericWidget::attachToEncoder() {
    bIsAttachedToEncoder = true;
}

void EncoderAttachedNumericWidget::detachFromEncoder() {
    bIsAttachedToEncoder = false;
}

void EncoderAttachedNumericWidget::setPosition(int xpos, int ypos) {
    this->x = xpos;
    this->y = ypos;
}

WidgetType EncoderAttachedNumericWidget::getType() const {
    return WidgetType::EncoderAttachedNumericWidget;
}