/**
 * @file TextLabelWidget.cpp
 * @brief Implements the TextLabelWidget class for displaying static text labels in the UI.
 *
 * This widget supports variable font sizes, optional use of the TomThumb font,
 * configurable colors, and an animated highlight underline effect.
 */

#include "widgets/Widget.h"
#include "widgets/TextLabelWidget.h"
#include <Fonts/TomThumb.h>
#include "Logging.h"
#include "ScreenManager.h"




/**
 * @brief Constructs a TextLabelWidget with the given label text and styling.
 *
 * @param t The text content of the label.
 * @param x X-coordinate of the label.
 * @param y Y-coordinate of the label.
 * @param fs Font size (ignored if `useTiny` is true).
 * @param tiny Whether to use the ultra-small TomThumb font.
 * @param c Color of the label text.
 */
TextLabelWidget::TextLabelWidget(const String& t, int x, int y,
                                 uint8_t fs,
                                 bool tiny,
                                 LabelColor c)
                                 : Widget(x, y, false)
    , text(t), fontSize(fs), useTinyFont(tiny), color(c) {
    log(LOG_VERBOSE, "Inside TextLabelWidget->constructor", __func__);
}


/**
 * @brief Renders the label text on the screen.
 *
 * Sets the font and color, positions the cursor, and draws the text.
 * If the widget is highlighted, an animated underline is toggled on each draw.
 */
void TextLabelWidget::draw() {
    log(LOG_VERBOSE, "Inside TextLabelWidget->draw()", __func__);
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
            ScreenManager::getDisplay()->drawLine(x, y + 6, x + text.length() * 6, y + 6, (uint16_t) LabelColor::WHITE);
        else
            ScreenManager::getDisplay()->drawLine(x, y + 6, x + text.length() * 6, y + 6, (uint16_t) LabelColor::BLACK);
    }
}

/**
 * @brief Handles hardware input. For this widget, no input is processed.
 */
void TextLabelWidget::handleInput() {
}


/**
 * @brief Updates the text displayed by the label.
 * @param newText New string to assign to the label.
 */
void TextLabelWidget::setText(const String& newText) {
    text = newText;
}


/**
 * @brief Changes the label's screen position.
 * @param xpos New x-coordinate.
 * @param ypos New y-coordinate.
 */
void TextLabelWidget::setPosition(int xpos, int ypos) {
    x = xpos;
    y = ypos;
}


/**
 * @brief Sets the font size used when `useTinyFont` is false.
 * @param fs New font size.
 */
void TextLabelWidget::setFontSize(uint8_t fs) {
    fontSize = fs;
}


/**
 * @brief Enables or disables the use of the TomThumb font.
 * @param tiny True to use the tiny font; false for default font.
 */
void TextLabelWidget::useTiny(bool tiny) {
    useTinyFont = tiny;
}


/**
 * @brief Sets the color of the label text.
 * @param c LabelColor enum value.
 */
void TextLabelWidget::setColor(LabelColor c) {
    color = c;
}


/**
 * @brief Returns the widget type as `WidgetType::TextLabel`.
 * @return WidgetType Enumeration value representing a text label.
 */
WidgetType TextLabelWidget::getType() const {
    return WidgetType::TextLabel;
}
