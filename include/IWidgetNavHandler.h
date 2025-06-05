/**
 * @file IWidgetNavHandler.h
 * @brief Interface for handling horizontal navigation events between UI widgets.
 *
 * This abstract class defines a contract for objects that need to respond when
 * navigation moves left or right out of a UI widget. It is intended to be
 * implemented by screens or containers that manage a set of navigable widgets.
 */

#pragma once


/**
 * @class IWidgetNavHandler
 * @brief Interface for responding to widget navigation events.
 *
 * Any class that implements this interface can react when a user attempts to
 * navigate beyond the left or right edge of the current widget focus area.
 */
class IWidgetNavHandler {
public:

    /**
     * @brief Called when the user navigates off the right edge of the current widget.
     */
    virtual void onLeavingWidgetRight() = 0;

    /**
     * @brief Called when the user navigates off the left edge of the current widget.
     */
    virtual void onLeavingWidgetLeft() = 0;

    /**
     * @brief Virtual destructor to ensure proper cleanup in derived classes.
     */
    virtual ~IWidgetNavHandler() = default;
};
