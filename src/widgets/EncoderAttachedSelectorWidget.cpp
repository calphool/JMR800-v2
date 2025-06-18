/**
 * @file EncoderAttachedSelectorWidget.h
 * @brief Widget for displaying and adjusting an array of string values
 */

#include "widgets/Widget.h"
#include "widgets/EncoderAttachedSelectorWidget.h"
#include "Logging.h"
#include "TypeCodes.h"
#include <ScreenManager.h>
#include <Fonts/TomThumb.h>


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

        if(toggle) {
            ScreenManager::getDisplay()->drawRect(x, y, rectwidth, 7, SH110X_BLACK);
            ScreenManager::getDisplay()->drawRect(x + width - 4, y, 4, 7, SH110X_BLACK);
            ScreenManager::getDisplay()->drawLine(x + 2, y, x + 2, y + 5, SH110X_BLACK);
            ScreenManager::getDisplay()->drawLine(x + 1, y + 4, x + 3, y + 4, SH110X_WHITE);
        } else {
            ScreenManager::getDisplay()->drawRect(x, y, rectwidth, 7, SH110X_WHITE);
            ScreenManager::getDisplay()->drawRect(x + width - 4, y, 4, 7, SH110X_WHITE);
            ScreenManager::getDisplay()->drawLine(x + 2, y, x + 2, y + 5, SH110X_WHITE);
            ScreenManager::getDisplay()->drawLine(x + 1, y + 4, x + 3, y + 4, SH110X_WHITE);
        }

        if(strlen(labels[currentValue]) == 0) {
            log(LOG_WARNING, "Current value label is empty", __func__);
            return;
        }
        
        ScreenManager::getDisplay()->setFont(&TomThumb);
        for(uint i = 0; i < strlen(labels[currentValue]); i++) {
            ScreenManager::getDisplay()->drawChar(x+(i*4)+2, y+2, labels[currentValue][i], 1, 0, 1);     
        }
        ScreenManager::getDisplay()->setFont();
    }

    void EncoderAttachedSelectorWidget::handleInput() {
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
    const char* EncoderAttachedSelectorWidget::getValue(uint index) {
        if(index >= labels.size())
            return nullptr;

        return labels[index];
    }
    
    /**
     * @brief Returns the widget type for this control.
     * @return WidgetType::EncoderAttachedNumericWidget
     */
    WidgetType getType() {
        return WidgetType::EncoderAttachedSelectorWidget;
    }


    /**
     * @brief Set the control's value
     */
    void EncoderAttachedSelectorWidget::addLabelAt(const char* s, uint index) {
        log(LOG_VERBOSE, "Inside EncoderAttachedSelectorWidget->setValue()", __func__);
        if (labels.size() <= index)
            labels.resize(index + 1, nullptr);

        labels[index] = s;

        this->recalculatePixelWidth();
    }


    void EncoderAttachedSelectorWidget::clearData() {
        log(LOG_VERBOSE, "Inside EncoderAttachedSelectorWidget->clearData()", __func__);
        labels.clear();
        currentValue = 0;
        width = 0;
    }
