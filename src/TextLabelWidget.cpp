#include "TextLabelWidget.h"
#include <Fonts/TomThumb.h>
#include "Logging.h"

TextLabelWidget::TextLabelWidget(Adafruit_SH1106G* disp,
                                 const String& t,
                                 int xpos,
                                 int ypos,
                                 uint8_t fs,
                                 bool tiny,
                                 LabelColor c)
    : display(disp), text(t), x(xpos), y(ypos), fontSize(fs), useTinyFont(tiny), color(c) {
    log(LOG_VERBOSE, "Inside TextLabelWidget->constructor");  
  }

void TextLabelWidget::draw() {
    log(LOG_VERBOSE, "Inside TextLabelWidget->draw()");  

    display->setCursor(x, y);
    if (useTinyFont) {
        display->setFont(&TomThumb);
    } else {
        display->setFont();
        display->setTextSize(fontSize);
    }
    display->setTextColor(static_cast<uint16_t>(color));
    display->print(text);
}

void TextLabelWidget::handleInput() {
    log(LOG_VERBOSE, "Inside TextLabelWidget->handleInput()");  
}

void TextLabelWidget::setText(const String& newText) {
    log(LOG_VERBOSE, "Inside TextLabelWidget->setText()");  
    text = newText;
}

void TextLabelWidget::setPosition(int xpos, int ypos) {
    log(LOG_VERBOSE, "Inside TextLabelWidget->setPosition()");  
    x = xpos;
    y = ypos;
}

void TextLabelWidget::setFontSize(uint8_t fs) {
    log(LOG_VERBOSE, "Inside TextLabelWidget->setFontSize()");  

    fontSize = fs;
}

void TextLabelWidget::useTiny(bool tiny) {
    log(LOG_VERBOSE, "Inside TextLabelWidget->useTiny()");  

    useTinyFont = tiny;
}

void TextLabelWidget::setColor(LabelColor c) {
    log(LOG_VERBOSE, "Inside TextLabelWidget->setColor()");  

    color = c;
}
