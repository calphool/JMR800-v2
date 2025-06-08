/**
 * @file KnobConfigDialog.h
 * @brief Modal dialog for editing knob assignments and configuration parameters.
 *
 * This dialog overlays the UI to allow users to configure a specific knob, offering
 * navigation between editable fields and saving or discarding changes.
 */

#pragma once

#include <vector>
#include "widgets/Widget.h"
#include "screens/ModalDialog.h"
#include "IWidgetNavHandler.h"
#include "widgets/TextInputWidget.h"
#include "widgets/PushButtonWidget.h"
#include "widgets/EncoderAttachedNumericWidget.h"

/**
 * @class KnobConfigDialog
 * @brief Modal dialog for configuring a knob’s control parameters.
 *
 * Allows editing of a knob’s assigned function or behavior. Implements both
 * modal lifecycle and widget navigation behaviors to manage field transitions.
 */
class KnobConfigDialog : public ModalDialog, public IWidgetNavHandler {

private:
    std::vector<Widget*> widgets;           ///< Widgets used in the dialog interface
    int xoffset;                            ///< X position of the dialog
    int yoffset;                            ///< Y position of the dialog
    int width;                              ///< Width of the dialog 
    int height;                             ///< Height of the dialog 
    bool done;                              ///< Whether the dialog is complete
    int active_knob;                        ///< ID of the knob being configured

    TextInputWidget* knobNameInputWidget = nullptr;
    EncoderAttachedNumericWidget* cmdByteWidget = nullptr;
    EncoderAttachedNumericWidget* typeCodeWidget = nullptr;
    PushButtonWidget* okButtonWidget = nullptr;
    PushButtonWidget* cancelButtonWidget = nullptr;

public:

    /**
     * @brief Constructs a new knob configuration dialog.
     * @param x X-coordinate of the dialog
     * @param y Y-coordinate
     * @param w Width of the dialog
     * @param h Height of the dialog
     * @param knob Knob ID to configure
     */
    KnobConfigDialog(int x, int y, int w, int h, int knob);

    /**
     * @brief Renders the dialog and all child widgets.
     */
    void draw() override;

    /**
     * @brief Returns whether the dialog has finished.
     * @return True if dialog should close
     */
    bool isDone() const;

    /**
     * @brief Lifecycle hook triggered when dialog becomes active.
     */
    void onEnter() override;

    /**
     * @brief Lifecycle hook triggered when dialog is dismissed.
     */
    void onExit() override;

    /**
     * @brief Handles user input events and routes them to widgets.
     */
    void handleInput() override;

    /**
     * @brief Handles rightward navigation beyond current widget.
     */
    void onLeavingWidgetRight() override;

    /**
     * @brief Handles leftward navigation beyond current widget.
     */
    void onLeavingWidgetLeft() override; 
};
