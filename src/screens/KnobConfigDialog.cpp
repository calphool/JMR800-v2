#include <cstring>
#include "screens/KnobConfigDialog.h"
#include "screens/ModalDialog.h"
#include "widgets/Widget.h"
#include "widgets/RectangleWidget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/TextInputWidget.h"
#include "widgets/EncoderAttachedSelectorWidget.h"
#include "widgets/PushButtonWidget.h"
#include "Logging.h"
#include "HardwareInterface.h"
#include "defines.h"

extern HardwareInterface* hardware;


    KnobConfigDialog::KnobConfigDialog(int x, int y, int w, int h, int knob) : ModalDialog(x, y, w, h) {
        xoffset = x;
        yoffset = y;
        width = w;
        height = h;
        active_knob = knob;
        done = false;
    }

    void KnobConfigDialog::setOnExitCallback(std::function<void()> callback) {
        onExitCallback = callback;
    }

    void KnobConfigDialog::onLeavingWidgetRight() {
        if(knobNameInputWidget && cmdByteWidget) {
            detachAllWidgets();
            cmdByteWidget->attachToEncoder();
            cmdByteWidget->setHighlighted(true);
        }
    }

    void KnobConfigDialog::onLeavingWidgetLeft() {
        if(knobNameInputWidget && cancelButtonWidget) {
            detachAllWidgets();
            cancelButtonWidget->attachToEncoder();
            cancelButtonWidget->setHighlighted(true);
        }
    }


    void KnobConfigDialog::draw() {
        for (Widget* w : widgets) {
                w->draw();
        }
    }

    bool KnobConfigDialog::isDone() const {
        return false;
    }

    void KnobConfigDialog::OkPressed() {
        if(okButtonWidget->getHighlightedStatus()) {
            log(LOG_INFO, "Ok was pressed", __func__);
            okButtonWidget->setOnPressCallback(nullptr);
            hardware->setKnobConfiguration(active_knob, knobNameInputWidget->getText(), cmdByteWidget->getValue(), typeCodeWidget->getValue());
            hardware->saveKnobs();
            if(onExitCallback) {
                this->onExitCallback();
                delay(500);
                hardware->clearEncoderButton();
            }
        }
    }

    void KnobConfigDialog::CancelPressed() {
        if(cancelButtonWidget->getHighlightedStatus()) {
            log(LOG_INFO, "Cancel was pressed", __func__);
            cancelButtonWidget->setOnPressCallback(nullptr);
            if(onExitCallback) {
                this->onExitCallback();
                delay(500);
                hardware->clearEncoderButton();
            }
        }
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

        knobNameInputWidget = new TextInputWidget(hardware->getKnobConfiguration(active_knob).name, xoffset + 2, yoffset + 12, width - 4, this);
        knobNameInputWidget->attachToEncoder();
        knobNameInputWidget->setHighlighted(true);
        widgets.push_back(knobNameInputWidget);

        Widget* textLabel2 = new TextLabelWidget(" Command Byte:", xoffset + 2, yoffset + 23, 1, false, LabelColor::WHITE);
        widgets.push_back(textLabel2);

        cmdByteWidget = new EncoderAttachedNumericWidget(xoffset + 90, yoffset + 23, 0, 255, buf);
        cmdByteWidget->setValue(hardware->getKnobConfiguration(active_knob).cmdbyte);
        cmdByteWidget->detachFromEncoder();
        widgets.push_back(cmdByteWidget);

        Widget* textLabel3 = new TextLabelWidget(" Type Code:", xoffset + 2, yoffset + 33, 1, false, LabelColor::WHITE);
        widgets.push_back(textLabel3);
        uint tcx = yoffset + 33;
        typeCodeWidget = new EncoderAttachedSelectorWidget(xoffset + 65, tcx);
        //typeCodeWidget = new EncoderAttachedNumericWidget(xoffset + 90, yoffset + 33, 0, 255, buf);
        typeCodeWidget->setValue(hardware->getKnobConfiguration(active_knob).typecode);
        typeCodeWidget->detachFromEncoder();
        widgets.push_back(typeCodeWidget);

        okButtonWidget = new PushButtonWidget("Ok", xoffset + 14, yoffset + height - 12);
        okButtonWidget->setOnPressCallback([this]() { this->OkPressed(); });
        widgets.push_back(okButtonWidget);

        cancelButtonWidget = new PushButtonWidget("Cancel", xoffset + width - 50, yoffset + height - 12);
        cancelButtonWidget->setOnPressCallback([this]() { this->CancelPressed(); });
        widgets.push_back(cancelButtonWidget);
    }
 
    void KnobConfigDialog::onExit() {
        for (Widget* widget : widgets) {
            delete widget;
        }
        widgets.clear();
        done = true;
    }

    void KnobConfigDialog::detachAllWidgets() {
        knobNameInputWidget->detachFromEncoder();
        cmdByteWidget->detachFromEncoder();
        typeCodeWidget->detachFromEncoder();
        okButtonWidget->detachFromEncoder();
        cancelButtonWidget->detachFromEncoder();
        knobNameInputWidget->setHighlighted(false);
        cmdByteWidget->setHighlighted(false);
        typeCodeWidget->setHighlighted(false);
        okButtonWidget->setHighlighted(false);
        cancelButtonWidget->setHighlighted(false);
    }

    void KnobConfigDialog::handleInput() {
        #ifndef TARGET_TEENSY
            static bool flip = true;
        #endif
        if(hardware->buttonStateChanged(1, true, true)) {  // moving right
            if(knobNameInputWidget && knobNameInputWidget->getHighlightedStatus()) {
                #ifndef TARGET_TEENSY
                flip = !flip;
                if(flip) return;
                #endif
                knobNameInputWidget->advanceCurrentPosition();
                return;
            } else if (cmdByteWidget && cmdByteWidget->getHighlightedStatus()) {
                detachAllWidgets();
                typeCodeWidget->setHighlighted(true);
                typeCodeWidget->attachToEncoder();
                return;
            } else if (typeCodeWidget && typeCodeWidget->getHighlightedStatus()) {
                detachAllWidgets();
                okButtonWidget->setHighlighted(true);
                okButtonWidget->attachToEncoder();
                return;
            } else if (okButtonWidget && okButtonWidget->getHighlightedStatus()) {
                detachAllWidgets();
                cancelButtonWidget->setHighlighted(true);
                cancelButtonWidget->attachToEncoder();
                return;
            } else if (cancelButtonWidget && cancelButtonWidget->getHighlightedStatus()) {
                detachAllWidgets();
                knobNameInputWidget->setHighlighted(true);
                knobNameInputWidget->setCurrentPosition(0);
                knobNameInputWidget->attachToEncoder(true);
                return;
            }
        } else if(hardware->buttonStateChanged(0, true, true)) { // moving left
            if(knobNameInputWidget && knobNameInputWidget->getHighlightedStatus()) {
                #ifndef TARGET_TEENSY
                flip = !flip;
                if(flip) return;
                #endif
                knobNameInputWidget->backtrackCurrentPosition();
                return;
            } else if (cmdByteWidget && cmdByteWidget->getHighlightedStatus()) {
                detachAllWidgets();
                knobNameInputWidget->setHighlighted(true);
                knobNameInputWidget->setCurrentPosition(knobNameInputWidget->getEndPosition());
                knobNameInputWidget->attachToEncoder(false);
                return;
            } else if(typeCodeWidget && typeCodeWidget->getHighlightedStatus()) {
                detachAllWidgets();
                cmdByteWidget->setHighlighted(true);
                cmdByteWidget->attachToEncoder();
                return;
            } else if(okButtonWidget && okButtonWidget->getHighlightedStatus()) {
                detachAllWidgets();
                typeCodeWidget->setHighlighted(true);
                typeCodeWidget->attachToEncoder();       
                return;         
            } else if(cancelButtonWidget && cancelButtonWidget->getHighlightedStatus()) {
                detachAllWidgets();
                okButtonWidget->setHighlighted(true);
                okButtonWidget->attachToEncoder();
                return;
            }
        }

        for (Widget* widget : widgets) {
            widget->handleInput();
        }
    }