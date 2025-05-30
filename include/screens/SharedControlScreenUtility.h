#pragma once

#include <vector>
#include "widgets/Widget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/RectangleWidget.h"
#include "widgets/PotentiometerKnobWidget.h"
#include "widgets/EncoderKnobWidget.h"
#include "widgets/ButtonWidget.h"
#include "Logging.h"

class SharedControlScreenUtility {
public:
    static void initializeControllerScreenLayout(std::vector<Widget*>& widgets, const char* labelText, TextLabelWidget*& outNumBox);
};
