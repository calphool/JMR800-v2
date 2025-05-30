    #include "widgets/Widget.h"
    #include "widgets/PushButtonWidget.h"
    #include "ScreenManager.h"
    #include "HardwareManager.h"
    
    
    PushButtonWidget::PushButtonWidget(const char* label, int x, int y) : Widget(x, y), pressed(false) {
        strcpy(text, label);
    }

    void PushButtonWidget::setPressed(bool state) {
        pressed = state;
    }

    bool PushButtonWidget::isAttachedToEncoder() {
        return bIsAttachedToEncoder;
    }

    void PushButtonWidget::attachToEncoder() {
        bIsAttachedToEncoder = true;
    }

    void PushButtonWidget::detachFromEncoder() {
        bIsAttachedToEncoder = false;
    }

    void PushButtonWidget::draw() {
        toggle = !toggle;

        int w = strlen(text) * 6 + 2;
        if(pressed)
            ScreenManager::getDisplay()->fillRect(x, y, w, 10, 0);
        else
            ScreenManager::getDisplay()->fillRect(x, y, w, 10, 1);

        if(bHighlighted) {
            if(toggle)
                ScreenManager::getDisplay()->drawRect(x, y, w, 10, 1);
            else
                ScreenManager::getDisplay()->drawRect(x, y, w, 10, 0);
        }
        
        ScreenManager::getDisplay()->setCursor(x+1, y+1);
        if(pressed) 
            ScreenManager::getDisplay()->setTextColor(1);
        else
            ScreenManager::getDisplay()->setTextColor(0);

        ScreenManager::getDisplay()->print(text);
        ScreenManager::getDisplay()->drawPixel(x,y,0);
        ScreenManager::getDisplay()->drawPixel(x+w-1,y,0);
        ScreenManager::getDisplay()->drawPixel(x,y+9,0);
        ScreenManager::getDisplay()->drawPixel(x+w-1,y+9,0);
    }

    void PushButtonWidget::handleInput() {
        if(bIsAttachedToEncoder) {
            if(hardware.getEncoderSwitchStatus()) {
                this->setPressed(true);
            }
            else {
                this->setPressed(false);
            }
        }
    }


    WidgetType PushButtonWidget::getType() const {
        return WidgetType::PushButton;
    }