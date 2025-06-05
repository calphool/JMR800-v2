/**
 * @file ModalDialog.h
 * @brief Abstract base class for modal dialogs in the JMR800-V2 UI.
 *
 * Modal dialogs overlay the current screen with a temporary UI element,
 * typically requiring user interaction before resuming normal operation.
 */

#pragma once

#include "screens/Screen.h"


/**
 * @class ModalDialog
 * @brief Base class for modal overlays requiring user dismissal or confirmation.
 *
 * Modal dialogs are interactive UI layers that block interaction with the
 * underlying screen until they are dismissed. Derived classes must implement
 * draw logic, lifecycle hooks, and a mechanism to indicate completion.
 */
class ModalDialog : public Screen {

private:
    int xoffset;    ///< Horizontal position of the dialog
    int yoffset;    ///< Vertical position of the dialog
    int width;      ///< Width of the dialog box
    int height;     ///< Height of the dialog box
    bool done;      ///< Completion flag (not used directly here, but reserved)

public:

    /**
     * @brief Constructs a modal dialog with specified position and dimensions.
     * @param x X-coordinate of the top-left corner
     * @param y Y-coordinate of the top-left corner
     * @param w Width of the dialog
     * @param h Height of the dialog
     */
  ModalDialog(int x, int y, int w, int h);

    /**
     * @brief Virtual destructor.
     */
  ~ModalDialog();

    /**
     * @brief Draws the dialog box on the screen.
     */
  virtual void draw() = 0;

    /**
     * @brief Indicates whether the dialog is finished and should be dismissed.
     * @return True if dialog is done
     */
  virtual bool isDone() const = 0;

    /**
     * @brief Called when the dialog becomes active.
     */
  virtual void onEnter() = 0;

    /**
     * @brief Called when the dialog is dismissed or replaced.
     */
  virtual void onExit() = 0;
};