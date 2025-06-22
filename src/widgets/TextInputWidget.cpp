
/**
 * @file TextInputWidget.cpp
 * @brief Implements the TextInputWidget class, allowing user-editable text display with cursor navigation.
 *
 * This widget provides interactive text input through button-based navigation and character replacement.
 * It supports cursor animation and optional external navigation handlers for moving between widgets.
 */
#ifdef TARGET_TEENSY
#include <Arduino.h>
#else
#include <stdint.h>
#endif
#include "widgets/TextInputWidget.h"
#include "widgets/Widget.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "IWidgetNavHandler.h"
#include "HardwareInterface.h"

extern HardwareInterface* hardware;



/**
 * @brief Constructs a TextInputWidget at a specified position with an editable character buffer.
 *
 * Pads the input string to fit the specified pixel width and ensures it fits the screen.
 * Optionally registers a navigation handler to signal when the cursor exits the widget.
 *
 * @param _text Editable character buffer to be displayed and modified.
 * @param x X-coordinate of the widget's top-left corner.
 * @param y Y-coordinate of the widget's top-left corner.
 * @param w Pixel width allocated for text display.
 * @param handler Optional navigation handler to notify on left/right exit.
 */
TextInputWidget::TextInputWidget(const char* _text, int x, int y, int w, IWidgetNavHandler* handler) : Widget(x, y, true), navHandler(handler) {
    currentPosition = 0;
    bIsAttachedToEncoder = false;
    strcpy(text, _text);
    width = w;
    int numChars = w / 6;
    this->x = x;
    this->y = y;
    
    while(((int)strlen(text)) < numChars) {
        strcat(text, " ");
    }

    if(((int)strlen(text) * 6) > w) {
        log(LOG_WARNING, "TextInputWidget: text is too long for the given width, truncating to fit", __func__);
        while(((int)strlen(text) * 6) > w && strlen(text) > 0) 
            text[strlen(text) - 1] = '\0'; 
    }

    hardware->resetEncoder(hardware->AsciiToEncoder(text[currentPosition]));
}

char* TextInputWidget::getText() {
    return text;
}


/**
 * @brief Renders the text input field and animated cursor.
 *
 * Draws each character at fixed spacing, adds an underline below each,
 * and toggles the cursor animation below the active character.
 */
void TextInputWidget::draw() {
    toggle = !toggle;

    for(int i = 0; i < (int)strlen(text); i++) {
        uint xpos = x + (i * 6);
        ScreenManager::getDisplay()->setCursor(xpos, y);
        ScreenManager::getDisplay()->print(text[i]);
        ScreenManager::getDisplay()->drawLine(xpos, y + 8,  xpos + 4, y + 8, SH110X_WHITE);
        if(currentPosition == i && bHighlighted) {
            if(toggle) 
                ScreenManager::getDisplay()->drawLine(xpos, y + 9, xpos + 4, y + 9, SH110X_WHITE);
            else 
                ScreenManager::getDisplay()->drawLine(xpos, y + 9, xpos + 4, y + 9, SH110X_BLACK);
        }
    }
}


/**
 * @brief Advances the cursor to the next character or invokes the navigation handler on overflow.
 */
void TextInputWidget::advanceCurrentPosition() {
    currentPosition++;
    if(currentPosition >= (int)strlen(text)) {
        if(navHandler) {
            navHandler->onLeavingWidgetRight();
        } 
        currentPosition = 0; 
    }
    else
        hardware->resetEncoder(hardware->AsciiToEncoder(text[currentPosition]));
}


/**
 * @brief Moves the cursor to the previous character or invokes the navigation handler on underflow.
 */
void TextInputWidget::backtrackCurrentPosition() {
    currentPosition--;
    if(currentPosition < 0) {
        if(navHandler) {
            navHandler->onLeavingWidgetLeft();
        }
        currentPosition = strlen(text) - 1;
    }
    else
        hardware->resetEncoder(hardware->AsciiToEncoder(text[currentPosition]));
}


/**
 * @brief Sets the character at the current cursor position.
 *
 * @param c Character to insert.
 */
/*
void TextInputWidget::setCharAtCurrentPosition(char c) {
    if(currentPosition < 0 || currentPosition >= (int)strlen(text)) {
        log(LOG_ERROR, "TextInputWidget: current position out of bounds", __func__);
        return;
    }
    text[currentPosition] = c; 
    text[strlen(text)] = '\0'; // Ensure null termination
}
*/

//char TextInputWidget::getCharAtCurrentPosition() {
//    return text[currentPosition];
//}

void TextInputWidget::attachToEncoder(bool bEnteringLeftEdge) {
    bIsAttachedToEncoder = true;
    if(bEnteringLeftEdge) 
        hardware->resetEncoder(hardware->AsciiToEncoder(text[0]));
    else
        hardware->resetEncoder(hardware->AsciiToEncoder(text[strlen(text) - 1]));
}

void TextInputWidget::detachFromEncoder() {
    bIsAttachedToEncoder = false; 
}

/**
 * @brief Handles hardware input for left/right button presses to navigate the cursor.
 */
void TextInputWidget::handleInput() {
    if(bIsAttachedToEncoder) {
        char charEncoderValue = hardware->getEncoderModdedBy(96) + 32;
        text[currentPosition] = charEncoderValue;
    }
}


void TextInputWidget::setCurrentPosition(uint8_t i) {
    currentPosition = i;
}

uint8_t TextInputWidget::getEndPosition() {
    return strlen(text) - 1;
}

/**
 * @brief Returns the type of widget for use in screen management or polymorphism.
 *
 * @return WidgetType::TextInputWidget
 */
WidgetType TextInputWidget::getType() const {
    return WidgetType::TextInputWidget;
}