/**
 * @file PushButtonWidget.cpp
 * @brief Implements the PushButtonWidget class, which represents a UI button with encoder interaction support.
 *
 * This widget simulates a pushbutton that can be toggled visually and respond to encoder button presses.
 * It supports highlighting, text labels, and press state rendering.
 */
#include "widgets/Widget.h"
#include "widgets/PushButtonWidget.h"
#include "ScreenManager.h"
#include "HardwareManager.h"
    

/**
 * @brief Constructs a PushButtonWidget with a label and position.
 *
 * @param label The text label displayed on the button.
 * @param x X-coordinate of the top-left corner of the button.
 * @param y Y-coordinate of the top-left corner of the button.
 */
PushButtonWidget::PushButtonWidget(const char* label, int x, int y) : Widget(x, y), pressed(false) {
    strcpy(text, label);
}


/**
 * @brief Sets the button's pressed state.
 *
 * @param state true if the button is pressed, false otherwise.
 */
void PushButtonWidget::setPressed(bool state) {
    pressed = state;
}


/**
 * @brief Returns whether this button is currently linked to encoder input.
 *
 * @return true if encoder is attached; false otherwise.
 */
bool PushButtonWidget::isAttachedToEncoder() {
    return bIsAttachedToEncoder;
}


/**
 * @brief Links this button to the encoder's push action.
 */
void PushButtonWidget::attachToEncoder() {
    bIsAttachedToEncoder = true;
}


/**
 * @brief Unlinks this button from encoder input.
 */
void PushButtonWidget::detachFromEncoder() {
    bIsAttachedToEncoder = false;
}


/**
 * @brief Draws the pushbutton widget on the screen.
 *
 * Renders a filled rectangle with inverted text color when pressed.
 * If highlighted, draws a flashing outline. Adds corner pixels for visual polish.
 */
void PushButtonWidget::draw() {
    toggle = !toggle;

    int w = strlen(text) * 6 + 2;
    if(pressed)
        ScreenManager::getDisplay()->fillRect(x, y, w, 10, 0);
    else
        ScreenManager::getDisplay()->fillRect(x, y, w, 10, 1);

    if(bHighlighted) {
        if(toggle)
            ScreenManager::getDisplay()->drawRect(x, y, w, 10, 1);
        else
            ScreenManager::getDisplay()->drawRect(x, y, w, 10, 0);
    }
    
    ScreenManager::getDisplay()->setCursor(x+1, y+1);
    if(pressed) 
        ScreenManager::getDisplay()->setTextColor(1);
    else
        ScreenManager::getDisplay()->setTextColor(0);

    ScreenManager::getDisplay()->print(text);
    ScreenManager::getDisplay()->drawPixel(x,y,0);
    ScreenManager::getDisplay()->drawPixel(x+w-1,y,0);
    ScreenManager::getDisplay()->drawPixel(x,y+9,0);
    ScreenManager::getDisplay()->drawPixel(x+w-1,y+9,0);
}


/**
 * @brief Handles hardware input.
 *
 * If the widget is attached to the encoder, updates the pressed state based on the encoder switch.
 */
void PushButtonWidget::handleInput() {
    if(bIsAttachedToEncoder) {
        if(hardware.getEncoderSwitchStatus()) {
            this->setPressed(true);
            if (onPressCallback) {
                //log(LOG_INFO, "invoking onPressCallback", __func__);
                onPressCallback(); // Execute the callback
            }
        }
        else {
            this->setPressed(false);
        }
    }
}


/**
 * @brief Sets a callback function to be invoked when the button is pressed.
 *
 * @return void
 */
void PushButtonWidget::setOnPressCallback(std::function<void()> callback) {
    onPressCallback = callback;
}



/**
 * @brief Returns the widget type identifier.
 *
 * @return WidgetType::PushButton
 */
WidgetType PushButtonWidget::getType() const {
    return WidgetType::PushButton;
}