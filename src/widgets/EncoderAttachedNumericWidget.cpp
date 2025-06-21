/**
 * @file EncoderAttachedNumericWidget.cpp
 * @brief UI widget displaying and controlling a numeric value using an encoder.
 *
 * This widget allows user input via a rotary encoder to adjust a numeric value
 * within a specified range. It supports highlighting, formatted display, and encoder binding.
 */

#include "widgets/EncoderAttachedNumericWidget.h"
#include "widgets/Widget.h"
#include "ScreenManager.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "HardwareInterface.h"

extern HardwareInterface* hardware;



/**
 * @brief Constructs an EncoderAttachedNumericWidget with positioning and value constraints.
 *
 * @param x X-coordinate of the widget on screen.
 * @param y Y-coordinate of the widget on screen.
 * @param min Minimum allowed value.
 * @param max Maximum allowed value.
 * @param _printFormat C-style format string used to render the numeric value.
 */
EncoderAttachedNumericWidget::EncoderAttachedNumericWidget(int x, int y, int min, int max, char* _printFormat) : Widget(x,y, false), 
    min(min), max(max)    
{
    control_value = 0;
    strcpy(printFormat, _printFormat);
};


/**
 * @brief Returns the current value of the widget.
 *
 * @return uint The current numeric value.
 */
uint EncoderAttachedNumericWidget::getValue() {
    return control_value;
}

void EncoderAttachedNumericWidget::setValue(int x) {
    control_value = x;
}


/**
 * @brief Renders the numeric value using the provided format string and highlights if active.
 */
void EncoderAttachedNumericWidget::draw() {
    char buff[20];
    toggle = !toggle;

    ScreenManager::getDisplay()->setCursor(x, y);
    sprintf(buff, printFormat, control_value);
    ScreenManager::getDisplay()->print(buff);

    if(bHighlighted) {
        if(toggle) 
            ScreenManager::getDisplay()->drawLine(x, y + 8, x + strlen(buff) * 5, y + 8, 0);
        else
            ScreenManager::getDisplay()->drawLine(x, y + 8, x + strlen(buff) * 5, y + 8, 1);
    }
}


/**
 * @brief Sets the control's value based on encoder input
 */
void EncoderAttachedNumericWidget::handleInput() {
    if(bIsAttachedToEncoder) {
        control_value = hardware->getEncoderZeroTo(max + min + 1) - min;
    }
}


/**
 * @brief Returns whether this widget is currently bound to encoder input.
 *
 * @return true if encoder is attached; false otherwise.
 */
bool EncoderAttachedNumericWidget::isAttachedToEncoder() {
    return bIsAttachedToEncoder;
}


/**
 * @brief Binds this widget to the encoder and initializes encoder state to match current value.
 */
void EncoderAttachedNumericWidget::attachToEncoder() {
    hardware->resetEncoder(control_value << 2);
    bIsAttachedToEncoder = true;
}


/**
 * @brief Unbinds this widget from encoder input.
 */
void EncoderAttachedNumericWidget::detachFromEncoder() {
    bIsAttachedToEncoder = false;
}


/**
 * @brief Sets the on-screen position of the widget.
 *
 * @param xpos X coordinate in pixels.
 * @param ypos Y coordinate in pixels.
 */
void EncoderAttachedNumericWidget::setPosition(int xpos, int ypos) {
    this->x = xpos;
    this->y = ypos;
}


/**
 * @brief Identifies the widget type for runtime dispatch or layout logic.
 *
 * @return WidgetType::EncoderAttachedNumericWidget
 */
WidgetType EncoderAttachedNumericWidget::getType() const {
    return WidgetType::EncoderAttachedNumericWidget;
}