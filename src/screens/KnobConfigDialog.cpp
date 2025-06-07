#include "screens/KnobConfigDialog.h"
#include "screens/ModalDialog.h"
#include "widgets/Widget.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/TextInputWidget.h"
#include "HardwareManager.h"
#include "widgets/EncoderAttachedNumericWidget.h"
#include "widgets/PushButtonWidget.h"


    KnobConfigDialog::KnobConfigDialog(int x, int y, int w, int h, int knob) : ModalDialog(x, y, w, h) {
        xoffset = x;
        yoffset = y;
        width = w;
        height = h;
        active_knob = knob;
        done = false;
    }

    void KnobConfigDialog::onLeavingWidgetRight() {
        hardware.setButtonLights(3, false, true);
    }

    void KnobConfigDialog::onLeavingWidgetLeft() {
        hardware.setButtonLights(3, true, false);
    }


    void KnobConfigDialog::draw() {
        for (Widget* w : widgets) {
                w->draw();
        }
    }

    bool KnobConfigDialog::isDone() const {
        return false;
    }

    void KnobConfigDialog::onEnter() {
        char buf[8];
        strcpy(buf,"%02x");
        Widget* blank = new RectangleWidget(xoffset, yoffset, width, height, true, RectColor::BLACK);
        widgets.push_back(blank);
        Widget* rectangle = new RectangleWidget(xoffset, yoffset, width, height, false, RectColor::WHITE); 
        widgets.push_back(rectangle);
        Widget* textLabel = new TextLabelWidget("Knob Name", (xoffset + (width/2))-27, yoffset + 2, 1, false, LabelColor::WHITE);
        widgets.push_back(textLabel);
        Widget* textInputWidget = new TextInputWidget(hardware.getKnobConfiguration(active_knob).name, xoffset + 2, yoffset + 12, width - 4, this);
        widgets.push_back(textInputWidget);
        Widget* textLabel2 = new TextLabelWidget(" Command Byte:", xoffset + 2, yoffset + 23, 1, false, LabelColor::WHITE);
        widgets.push_back(textLabel2);
        EncoderAttachedNumericWidget* eanw = new EncoderAttachedNumericWidget(xoffset + 90, yoffset + 23, 0, 255, buf);
        eanw->setValue(hardware.getKnobConfiguration(active_knob).cmdbyte);
        eanw->detachFromEncoder();
        widgets.push_back(eanw);
        Widget* textLabel3 = new TextLabelWidget(" Type Code:", xoffset + 2, yoffset + 33, 1, false, LabelColor::WHITE);
        widgets.push_back(textLabel3);
        EncoderAttachedNumericWidget* eanw2 = new EncoderAttachedNumericWidget(xoffset + 90, yoffset + 33, 0, 255, buf);
        eanw2->setValue(hardware.getKnobConfiguration(active_knob).typecode);
        eanw2->detachFromEncoder();
        widgets.push_back(eanw2);
        PushButtonWidget* b1 = new PushButtonWidget("Ok", xoffset + 14, yoffset + height - 12);
        widgets.push_back(b1);
        PushButtonWidget* b2 = new PushButtonWidget("Cancel", xoffset + width - 50, yoffset + height - 12);
        widgets.push_back(b2);
    }
 
    void KnobConfigDialog::onExit() {
        for (Widget* widget : widgets) {
            delete widget;
        }
        widgets.clear();
        done = true;
    }

    void KnobConfigDialog::handleInput() {
        for (Widget* widget : widgets) {
            widget->handleInput();
        }
    }