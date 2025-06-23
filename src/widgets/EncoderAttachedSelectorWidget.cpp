/**
 * @file EncoderAttachedSelectorWidget.h
 * @brief Widget for displaying and adjusting an array of string values
 */

#include <cstring>
#include <cstdint>
#include "widgets/Widget.h"
#include "widgets/EncoderAttachedSelectorWidget.h"
#include "Logging.h"
#include "TypeCodes.h"
#include <ScreenManager.h>
#ifndef TARGET_TEENSY
#define SH110X_BLACK 0
#define SH110X_WHITE 1
#endif
#include "HardwareInterface.h"

extern HardwareInterface* hardware;



    EncoderAttachedSelectorWidget::EncoderAttachedSelectorWidget(int x, int y) : Widget(x, y) {
        log(LOG_VERBOSE, "Inside EncoderAttachedSelectorWidget->constructor", __func__);

        // default array of strings for the encoder selector
        for(uint i =0; i < NUM_TYPE_CODES; i++) {
            this->addLabelAt(typeCodes[i].typeCodeName, typeCodes[i].cd);
        }
        currentValue = 0;
        this->recalculatePixelWidth();
    }

    void EncoderAttachedSelectorWidget::recalculatePixelWidth() {
        log(LOG_VERBOSE, "Inside EncoderAttachedSelectorWidget->recalculatePixelWidth()", __func__);
        width = 0;
        for(uint i = 0; i < labels.size(); i++) {
            if(labels[i] == nullptr) {
                continue;
            }
            if((4*strlen(labels[i])) > width) {
                width = (4*strlen(labels[i]));
            }
        }
    }

    void EncoderAttachedSelectorWidget::draw() {
        // TomThumb font is 3x5 pixels per character
        toggle = !toggle;
        log(LOG_VERBOSE, "Inside EncoderAttachedSelectorWidget->draw()", __func__);

        if (labels.empty()) {
            log(LOG_WARNING, "No labels set for EncoderAttachedSelectorWidget", __func__);
            return;
        }

        uint rectwidth = width + 8; // 1 character for the down arrow, and 2 pixels padding on each side

        if(bHighlighted) {
            if(toggle) {
                ScreenManager::getDisplay()->drawRect(x, y, rectwidth, 9, SH110X_BLACK);
                ScreenManager::getDisplay()->drawRect(x + rectwidth - 6, y, 6, 9, SH110X_BLACK);
                ScreenManager::getDisplay()->drawLine(x + rectwidth - 7, y, x + rectwidth - 4, y + 5, SH110X_BLACK);
                ScreenManager::getDisplay()->drawLine(x + rectwidth - 3, y + 5, x + rectwidth - 1, y, SH110X_BLACK);
            } else {
                ScreenManager::getDisplay()->drawRect(x, y, rectwidth, 9, SH110X_WHITE);
                ScreenManager::getDisplay()->drawRect(x + rectwidth - 6, y, 6, 9, SH110X_WHITE);
                ScreenManager::getDisplay()->drawLine(x + rectwidth - 7, y, x + rectwidth - 4, y + 5, SH110X_WHITE);
                ScreenManager::getDisplay()->drawLine(x + rectwidth - 3, y + 5, x + rectwidth - 1, y, SH110X_WHITE);
            }
        }

        if(strlen(labels[currentValue]) == 0) {
            log(LOG_WARNING, "Current value label is empty", __func__);
            return;
        }
        
        ScreenManager::getDisplay()->setFont(FontSize::Small);
        for(uint i = 0; i < strlen(labels[currentValue]); i++) {
            ScreenManager::getDisplay()->drawChar((uint16_t) x+(i*4)+2, (uint16_t) y+7, 
            labels[currentValue][i], SH110X_WHITE, SH110X_BLACK);
        }
        ScreenManager::getDisplay()->setFont();
    }

    void EncoderAttachedSelectorWidget::handleInput() {
        if(bIsAttachedToEncoder) {
            currentValue = hardware->getEncoderZeroTo(labels.size());
        }
    }

    void EncoderAttachedSelectorWidget::setValue(uint8_t x) {
        currentValue = x;
    }

 
    /**
     * @brief Enables encoder control for this widget.
     */
    void EncoderAttachedSelectorWidget::attachToEncoder() {
        log(LOG_VERBOSE, "Inside EncoderAttachedSelectorWidget->attachToEncoder()", __func__);
        bIsAttachedToEncoder = true;
    }
    
    /**
     * @brief Disables encoder control for this widget.
     */
    void EncoderAttachedSelectorWidget::detachFromEncoder() {
        log(LOG_VERBOSE, "Inside EncoderAttachedSelectorWidget->detachFromEncoder()", __func__);
        bIsAttachedToEncoder = false;
    }
    
    /**
     * @brief Indicates whether the widget currently has encoder focus.
     * @return True if attached
     */
    bool EncoderAttachedSelectorWidget::isAttachedToEncoder() {
        return bIsAttachedToEncoder;
    }
    
    /**
     * @brief Gets the current numeric value.
     * @return Current value
     */
    uint8_t EncoderAttachedSelectorWidget::getValue() {
        return currentValue;
    }
    
    /**
     * @brief Returns the widget type for this control.
     * @return WidgetType::EncoderAttachedNumericWidget
     */
    WidgetType EncoderAttachedSelectorWidget::getType() const {
        return WidgetType::EncoderAttachedSelectorWidget;
    }


    /**
     * @brief Set the control's value
     */
    void EncoderAttachedSelectorWidget::addLabelAt(const char* s, uint8_t index) {
        log(LOG_VERBOSE, "Inside EncoderAttachedSelectorWidget->setValue()", __func__);
        if (labels.size() <= index)
            labels.resize(index + 1, nullptr);

        labels[index] = s;

        this->recalculatePixelWidth();
    }

/*
    void EncoderAttachedSelectorWidget::clearData() {
        log(LOG_VERBOSE, "Inside EncoderAttachedSelectorWidget->clearData()", __func__);
        labels.clear();
        currentValue = 0;
        width = 0;
    }
*/
