#include "widgets/Widget.h"
#include "widgets/TextLabelWidget.h"
#include <Fonts/TomThumb.h>
#include "Logging.h"
#include "ScreenManager.h"

TextLabelWidget::TextLabelWidget(const String& t, int x, int y,
                                 uint8_t fs,
                                 bool tiny,
                                 LabelColor c)
                                 : Widget(x, y)
    , text(t), fontSize(fs), useTinyFont(tiny), color(c) {
    log(LOG_VERBOSE, "Inside TextLabelWidget->constructor");
}

void TextLabelWidget::draw() {
    log(LOG_VERBOSE, "Inside TextLabelWidget->draw()");  

    ScreenManager::getDisplay()->setCursor(x, y);
    if (useTinyFont) {
        ScreenManager::getDisplay()->setFont(&TomThumb);
    } else {
        ScreenManager::getDisplay()->setFont();
        ScreenManager::getDisplay()->setTextSize(fontSize);
    }
    ScreenManager::getDisplay()->setTextColor(static_cast<uint16_t>(color));
    ScreenManager::getDisplay()->print(text);
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

WidgetType TextLabelWidget::getType() const {
    return WidgetType::TextLabel;
}
