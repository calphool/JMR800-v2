/**
 * @file Widget.h
 * @brief Abstract base class for UI widgets in the JMR800-V2 OLED interface.
 *
 * Provides a consistent interface and positioning logic for all screen widgets.
 * Derived classes must implement `draw()` and `handleInput()`, and identify
 * their type via `getType()`.
 */

#pragma once


/**
 * @enum WidgetType
 * @brief Enumerates all known widget types for classification and rendering.  This is used as an alternative 
 * to RTTI (Run-Time Type Information) to avoid the overhead of dynamic_cast.
 */
enum class WidgetType {
    Unknown,                        ///< Default or uninitialized type
    Rectangle,                      ///< Simple filled or outlined rectangle
    Knob,                           ///< Logical knob control (parent of PotentiometerKnob and EncoderKnob)
    PotentiometerKnob,              ///< Physical knob read via ADC
    EncoderKnob,                    ///< Rotary encoder with button functionality
    TextLabel,                      ///< Static text label widget
    Button,                         ///< Represents a physical button with LED indicators
    EncoderAttachedNumericWidget,   ///< Numeric input widget using an encoder
    PushButton,                     ///< UI button with push functionality (like an OK or Cancel UI button)
    TextInputWidget,                ///< Editable text input field
    StarField,                      ///< StarField widget (sort of useless, but it was fun to add)
    EncoderAttachedSelectorWidget   ///< Selector widget that allows choosing from a list of options using an encoder
};


/**
 * @class Widget
 * @brief Base class for all on-screen widgets.
 *
 * Widgets define position, drawing, and input handling behavior for screen elements.
 * Derived classes must implement `draw()` and `handleInput()`.
 */
class Widget {
public:

    /**
     * @brief Constructs a widget at a specified screen location.
     * @param x X-coordinate in pixels
     * @param y Y-coordinate in pixels
     */
    Widget(int x, int y);

    /**
     * @brief Constructs a widget with optional highlighting state.
     * @param x X-coordinate
     * @param y Y-coordinate
     * @param highlighted If true, widget is initially highlighted
     */
    Widget(int x, int y, bool highlighted);

    /**
     * @brief Draws the widget on the display. Must be overridden.
     */
    virtual void draw() = 0;

    /**
     * @brief Handles user input specific to this widget.
     */
    virtual void handleInput() = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~Widget() {}



    /**
     * @brief Sets the highlighting state for this widget.
     * @param b True to highlight, false to clear highlighting.
     */
    void setHighlighted(bool b);

    /**
     * @brief Returns the widget type for runtime introspection.
     * @return Enum value identifying the widget type.
     */
    virtual WidgetType getType() const = 0;

    /**
     * @brief Returns the widget highlight status
     * @return the widget highlight status
     */
    bool getHighlightedStatus();

protected:
    int x; ///< Horizontal position in pixels
    int y; ///< Vertical position in pixels
    bool bHighlighted; ///< Highlight flag for focus or selection
    bool toggle; ///< Optional flag for toggle-style widgets

};
