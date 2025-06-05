#pragma once

class IWidgetNavHandler {
public:
    virtual void onLeavingWidgetRight() = 0;
    virtual void onLeavingWidgetLeft() = 0;
    virtual ~IWidgetNavHandler() = default;
};
