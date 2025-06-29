#pragma once

#include <vector>
#include <functional>
#include "widgets/Widget.h"
#include "screens/ModalDialog.h"
#include "IWidgetNavHandler.h"
#include "widgets/TextInputWidget.h"
#include "widgets/EncoderAttachedNumericWidget.h"
#include "widgets/EncoderAttachedSelectorWidget.h"
#include "widgets/PushButtonWidget.h"

class KnobConfigDialog : public ModalDialog, public IWidgetNavHandler {
private:
    /* geometry */
    int  xoffset;
    int  yoffset;
    int  width;
    int  height;

    /* state */
    bool done {false};                 //!< becomes true when OK or Cancel pressed
    int  active_knob {0};

    /* widgets */
    std::vector<Widget*> widgets;
    TextInputWidget*               knobNameInputWidget   {nullptr};
    EncoderAttachedNumericWidget*  cmdByteWidget         {nullptr};
    EncoderAttachedSelectorWidget* typeCodeWidget        {nullptr};
    PushButtonWidget*              okButtonWidget        {nullptr};
    PushButtonWidget*              cancelButtonWidget    {nullptr};

    std::function<void()> onExitCallback {nullptr};

    /* helper */
    void detachAllWidgets();
    void OkPressed();
    void CancelPressed();

public:
    KnobConfigDialog(int x, int y, int w, int h, int knob);

    /* ModalDialog overrides */
    void draw() override;
    void onEnter() override;
    void onExit()  override;
    void handleInput() override;
    bool isDone() const;

    /* IWidgetNavHandler overrides */
    void onLeavingWidgetRight() override;
    void onLeavingWidgetLeft()  override;

    /* external hooks */
    void setOnExitCallback(std::function<void()> cb);
};