/**
 * @file Screen.h
 * @brief Abstract interface for all UI screens in the JMR800-V2 system.
 *
 * This class defines the basic lifecycle and input-handling structure for screen objects
 * that are rendered on the OLED display. Concrete implementations should inherit from
 * this class and override the necessary virtual methods.
 */

#pragma once


/**
 * @class Screen
 * @brief Base class for user interface screens.
 *
 * This interface defines a screen as a unit of UI behavior, capable of rendering its
 * own display and responding to input. Optional lifecycle hooks allow for setup and
 * cleanup when a screen is entered or exited.
 */
class Screen {
public:

    /**
     * @brief Renders the screen contents to the display.
     */
    virtual void draw() = 0;

    /**
     * @brief Handles input events specific to the screen.
     */
    virtual void handleInput() = 0;

    /**
     * @brief Optional hook called when the screen becomes active.
     *
     * Can be used to reset visual state, initialize data, or begin animations.
     */
    virtual void onEnter() {}  // Optional: load state

    /**
     * @brief Optional hook called when the screen is about to be replaced.
     *
     * Can be used to save state or clean up resources.
     */
    virtual void onExit() {}   // Optional: cleanup or save

    /**
     * @brief Virtual destructor.
     */
    virtual ~Screen() {}
};

