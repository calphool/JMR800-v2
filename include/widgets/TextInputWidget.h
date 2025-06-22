/**
 * @file TextInputWidget.h
 * @brief Editable text input field for alphanumeric data entry on the OLED interface.
 *
 * This widget allows user input through character selection and cursor movement.
 * It supports fixed-width fields and integrates with navigation handlers for
 * screen-level focus transitions.
 */

#pragma once

#ifdef TARGET_TEENSY
#include <Arduino.h>
#else
#include <stdint.h>
#endif
#include "widgets/Widget.h"
#include "IWidgetNavHandler.h"

/**
 * @class TextInputWidget
 * @brief Widget that displays and allows editing of a fixed-length text string.
 *
 * Users can move a cursor across the text field and change individual characters.
 * An optional IWidgetNavHandler allows the widget to signal when navigation
 * moves beyond its bounds.
 */
class TextInputWidget : public Widget {

private:
    char text[25];              ///< Editable text buffer
    int currentPosition;        ///< Cursor position within the buffer
    int width;                  ///< Visible field width (in pixels)
    IWidgetNavHandler* navHandler = nullptr;  ///< Optional handler for left/right navigation out of field
    bool bIsAttachedToEncoder;

public:

    /**
     * @brief Constructs a new TextInputWidget.
     * @param _text Initial null-terminated string to load into the field
     * @param x X position in pixels
     * @param y Y position in pixels
     * @param w Width of the editable field (in characters)
     * @param navHandler Optional handler for off-edge navigation
     */
    TextInputWidget(const char* _text, int x, int y, int w, IWidgetNavHandler* handler = nullptr);

    char *getText();

    /**
     * @brief Draws the text field and highlights the active cursor position.
     */
    void draw() override;

    /**
     * @brief Processes input events to move cursor or change characters.
     */
    void handleInput() override;

    /**
     * @brief Advances the cursor to the next character position.
     */
    void advanceCurrentPosition();

    /**
     * @brief Moves the cursor to the previous character position.
     */
    void backtrackCurrentPosition();

    /**
     * @brief Updates the character at the current cursor position.
     * @param c New character to set
     */
    //void setCharAtCurrentPosition(char c);

    //char getCharAtCurrentPosition();

    void attachToEncoder(bool bEnteringLeftEdge = true);

    void detachFromEncoder();

    void setCurrentPosition(uint8_t i);

    uint8_t getEndPosition();

    /**
     * @brief Returns the widget type as WidgetType::TextInputWidget.
     * @return WidgetType enum value
     */
    WidgetType getType() const override;
};