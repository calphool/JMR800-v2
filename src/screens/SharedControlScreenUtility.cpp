/**
 * @file SharedControlScreenUtility.cpp
 * @brief Provides layout helper utilities for constructing shared UI control screens.
 */

#include "screens/SharedControlScreenUtility.h"
#include "TeensyHardwareManager.h"


/**
 * @brief Initializes a default controller screen layout using a variety of widgets.
 *
 * This method populates a widget list with a standard arrangement of knobs, buttons,
 * labels, and decorative rectangles. It positions widgets based on a fixed pixel grid
 * and prepares the output pointer `outNumBox` for later dynamic value display.
 *
 * @param widgets Reference to a vector of Widget pointers to populate.
 * @param labelText A centered label string to display at the top of the screen.
 * @param outNumBox Reference to a pointer that will be assigned a `TextLabelWidget` representing a numeric display box.
 */
void SharedControlScreenUtility::initializeControllerScreenLayout(std::vector<Widget*>& widgets, const char* labelText, TextLabelWidget*& outNumBox) {
    log(LOG_VERBOSE, "Inside WidgetLayoutHelper::initializeTestScreenLayout()", __func__);

    int textWidth = strlen(labelText) * 6;
    int x = (SCREEN_WIDTH - textWidth) / 2;

    widgets.push_back(new RectangleWidget(0, 9, SCREEN_WIDTH, 54, false, RectColor::WHITE));
    widgets.push_back(new TextLabelWidget(labelText, x, 0, 1, false));

    outNumBox = new TextLabelWidget("---", 90, 17, 1, false, LabelColor::WHITE);
    widgets.push_back(outNumBox);

    int iKnobID = 0;
    for(int i=7; i<78;i=i+10) { 
        for(int j=14; j<62;j = j + 8) {
            widgets.push_back(new PotentiometerKnobWidget(iKnobID++, i, j));
        }
    }

    for(int i=87; i < 98; i = i + 10) {
        for(int j=30; j < 62; j = j + 8) {
            widgets.push_back(new PotentiometerKnobWidget(iKnobID++, i, j));
        }
    }

    widgets.push_back(new EncoderKnobWidget(109,18));
    widgets.push_back(new RectangleWidget(88, 15, 21, 11, false, RectColor::WHITE));

    int i=0;
    for(int j=30; j < 62; j = j + 8) {
        widgets.push_back(new ButtonWidget(i, 109, j));
        i++;
    }
}
