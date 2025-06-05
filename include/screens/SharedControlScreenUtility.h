/**
 * @file SharedControlScreenUtility.h
 * @brief Provides utility functions for initializing common screen layouts.
 *
 * This utility class contains static methods to assist in the setup of reusable
 * control layouts involving widgets like labels, knobs, and buttons.
 */

#pragma once

#include <vector>
#include "widgets/Widget.h"
#include "widgets/TextLabelWidget.h"
#include "widgets/RectangleWidget.h"
#include "widgets/PotentiometerKnobWidget.h"
#include "widgets/EncoderKnobWidget.h"
#include "widgets/ButtonWidget.h"
#include "Logging.h"


/**
 * @class SharedControlScreenUtility
 * @brief Static utility class for populating screen widget layouts.
 *
 * This class is used to encapsulate layout logic that is common across multiple
 * screens. It supports setting up consistent widget arrangements and references.
 */
class SharedControlScreenUtility {

public:
    /**
     * @brief Initializes a screen layout with standard controller widgets.
     *
     * Populates the given widget vector with positioned UI elements, including
     * a labeled numeric display box and other screen controls.
     *
     * @param widgets Vector of Widget pointers to populate
     * @param labelText Text to display in the label widget
     * @param outNumBox Reference to a pointer that will be assigned the labeled output box widget
     */
    static void initializeControllerScreenLayout(std::vector<Widget*>& widgets, const char* labelText, TextLabelWidget*& outNumBox);
};
