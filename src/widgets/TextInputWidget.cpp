
#include "widgets/TextInputWidget.h"
#include "widgets/Widget.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"



TextInputWidget::TextInputWidget(char* _text, int x, int y, int w) : Widget(x, y, true) {
    currentPosition = 0;
    strcpy(text, _text);
    width = w;
    int numChars = w / 6;
    this->x = x;
    this->y = y;
    
    
    while(strlen(text) < numChars) {
        strcat(text, " ");
    }

    if(strlen(text) * 6 > w) {
        log(LOG_WARNING, "TextInputWidget: text is too long for the given width, truncating to fit");
        while(strlen(text) * 6 > w && strlen(text) > 0) 
            text[strlen(text) - 1] = '\0'; 
    }
}


void TextInputWidget::draw() {
    toggle = !toggle;

    for(int i = 0; i < strlen(text); i++) {
        uint xpos = x + (i * 6);
        ScreenManager::getDisplay()->setCursor(xpos, y);
        ScreenManager::getDisplay()->print(text[i]);
            ScreenManager::getDisplay()->drawLine(xpos, y + 8,  xpos + 5, y + 8, SH110X_WHITE);
        if(currentPosition == i) {
            if(toggle) 
                ScreenManager::getDisplay()->drawLine(xpos, y + 10, xpos + 5, y + 10, SH110X_WHITE);
            else 
                ScreenManager::getDisplay()->drawLine(xpos, y + 10, xpos + 5, y + 10, SH110X_BLACK);
        }
    }
}


void TextInputWidget::advanceCurrentPosition() {
    currentPosition++;
    if(currentPosition >= strlen(text)) {
        currentPosition = 0;
    }
}

void TextInputWidget::backtrackCurrentPosition() {
    currentPosition--;
    if(currentPosition < 0) {
        currentPosition = strlen(text) - 1;
    }
}

void TextInputWidget::setCharAtCurrentPosition(char c) {
    if(currentPosition < 0 || currentPosition >= strlen(text)) {
        log(LOG_ERROR, "TextInputWidget: current position out of bounds");
        return;
    }
    text[currentPosition] = c;
    text[strlen(text)] = '\0'; // Ensure null termination
}

void TextInputWidget::handleInput() {
    if(hardware.isButtonPressed(0)) { // Assuming button 0 is the advance button
        advanceCurrentPosition();
    } else if(hardware.isButtonPressed(1)) { // Assuming button 1 is the backtrack button
        backtrackCurrentPosition();
    }
}

WidgetType TextInputWidget::getType() const {
    return WidgetType::TextInputWidget;
}