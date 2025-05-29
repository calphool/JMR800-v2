#include "widgets/Widget.h"
#include "widgets/TextLabelWidget.h"
#include <Fonts/TomThumb.h>
#include "Logging.h"
#include "ScreenManager.h"

TextLabelWidget::TextLabelWidget(const String& t, int x, int y,
                                 uint8_t fs,
                                 bool tiny,
                                 LabelColor c)
                                 : Widget(x, y, false)
    , text(t), fontSize(fs), useTinyFont(tiny), color(c) {
    log(LOG_VERBOSE, "Inside TextLabelWidget->constructor");
}

void TextLabelWidget::draw() {
    log(LOG_VERBOSE, "Inside TextLabelWidget->draw()");
    toggle = !toggle;

    ScreenManager::getDisplay()->setCursor(x, y);
    if (useTinyFont) {
        ScreenManager::getDisplay()->setFont(&TomThumb);
    } else {
        ScreenManager::getDisplay()->setFont();
        ScreenManager::getDisplay()->setTextSize(fontSize);
    }
    ScreenManager::getDisplay()->setTextColor(static_cast<uint16_t>(color));
    ScreenManager::getDisplay()->print(text);

    if(bHighlighted) {
        if(toggle)
            ScreenManager::getDisplay()->drawLine(x, y + 6, x + text.length() * 6, y+6, (uint16_t) LabelColor::WHITE);
        else
            ScreenManager::getDisplay()->drawLine(x, y + 6, x + text.length() * 6, y+6, (uint16_t) LabelColor::BLACK);
    }
}

void TextLabelWidget::handleInput() {
}

void TextLabelWidget::setText(const String& newText) {
    text = newText;
}

void TextLabelWidget::setPosition(int xpos, int ypos) {
    x = xpos;
    y = ypos;
}

void TextLabelWidget::setFontSize(uint8_t fs) {
    fontSize = fs;
}

void TextLabelWidget::useTiny(bool tiny) {
    useTinyFont = tiny;
}

void TextLabelWidget::setColor(LabelColor c) {
    color = c;
}

WidgetType TextLabelWidget::getType() const {
    return WidgetType::TextLabel;
}
