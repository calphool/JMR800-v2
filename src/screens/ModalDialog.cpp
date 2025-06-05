/**
 * @file ModalDialog.cpp
 * @brief Implements a base modal dialog screen used to temporarily take over UI control.
 *
 * ModalDialog is intended to be subclassed for specific types of modals, such as confirmations,
 * alerts, or temporary menus. It provides positioning, size, and completion-state tracking.
 */

#include "screens/ModalDialog.h"
#include "widgets/RectangleWidget.h"


/**
 * @brief Constructs a ModalDialog with specified position and dimensions.
 *
 * @param x X-coordinate offset of the dialog box.
 * @param y Y-coordinate offset of the dialog box.
 * @param w Width of the dialog box in pixels.
 * @param h Height of the dialog box in pixels.
 */
ModalDialog::ModalDialog(int x, int y, int w, int h) : xoffset(x), yoffset(y), width(w), height(h) {
    done = false;
}


/**
 * @brief Virtual destructor for ModalDialog. Allows safe subclassing.
 */
ModalDialog::~ModalDialog() {
}

/**
 * @brief Called when the modal dialog becomes active.
 *
 * Typically used to initialize dialog state or screen elements.
 */
void ModalDialog::onEnter() {
}

/**
 * @brief Called when the modal dialog is dismissed or replaced.
 *
 * Override to clean up or store transient state.
 */
void ModalDialog::onExit() {
}


/**
 * @brief Draws the modal dialog to the screen.
 *
 * Override in subclasses to implement actual visual layout and content.
 */
void ModalDialog::draw() {
}

/**
 * @brief Returns true if the dialog has completed and can be closed.
 *
 * @return true if done, false otherwise.
 */
bool ModalDialog::isDone()const  {
    return done;
}