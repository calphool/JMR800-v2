/**
 * @file TextLabelWidget.h
 * @brief Displays static or dynamic text labels on the OLED UI.
 *
 * Supports standard and tiny font rendering using Adafruit GFX fonts.
 * This widget does not respond to input events, but can be dynamically
 * updated and redrawn.
 */
#pragma once

#include "widgets/Widget.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

extern const GFXfont TomThumb;  ///< Tiny pixel font used when useTinyFont is enabled

/**
 * @enum LabelColor
 * @brief Text rendering color for OLED display.
 */
enum class LabelColor : uint8_t {
    BLACK = SH110X_BLACK,
    WHITE = SH110X_WHITE
};


/**
 * @class TextLabelWidget
 * @brief Widget for displaying text labels with optional font size and style.
 *
 * This widget renders static or updated text on the screen. Supports positioning,
 * font scaling, and color inversion for label emphasis.
 */
class TextLabelWidget : public Widget {
private:
    String text;            ///< Text to render
    uint8_t fontSize;       ///< Font size multiplier (standard font)
    bool useTinyFont;       ///< Whether to use the TomThumb pixel font
    LabelColor color;       ///< Text color (white or black)

public:

    /**
     * @brief Constructs a text label widget.
     * @param t Initial text content
     * @param x Horizontal position (pixels)
     * @param y Vertical position (pixels)
     * @param fs Font size (default: 1)
     * @param tiny If true, uses tiny font instead of Adafruit default
     * @param color Text color (default: white)
     */
    TextLabelWidget(const String& t, int x, int y, uint8_t fs = 1, bool tiny = false, 
        LabelColor color = LabelColor::WHITE);

    /**
     * @brief Draws the label text to the OLED display.
     */
    void draw() override;

    /**
     * @brief No-op input handler for compatibility with Widget interface.
     */
    void handleInput() override;

    /**
     * @brief Updates the label’s text.
     * @param newText New text to display
     */
    void setText(const String& newText);

    /**
     * @brief Changes the widget’s screen position.
     * @param xpos New X-coordinate
     * @param ypos New Y-coordinate
     */
    void setPosition(int xpos, int ypos);

    /**
     * @brief Changes the font size.
     * @param fs New font size multiplier
     */
    void setFontSize(uint8_t fs);

    /**
     * @brief Enables or disables the tiny font.
     * @param tiny True to use TomThumb font
     */
    void useTiny(bool tiny);

    /**
     * @brief Sets the text rendering color.
     * @param c New label color
     */
    void setColor(LabelColor c);

    /**
     * @brief Identifies the widget as a TextLabel type.
     * @return WidgetType::TextLabel
     */
    WidgetType getType() const override;
};
