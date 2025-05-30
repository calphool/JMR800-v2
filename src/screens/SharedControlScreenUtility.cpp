#include "screens/SharedControlScreenUtility.h"
#include "HardwareManager.h"

void SharedControlScreenUtility::initializeControllerScreenLayout(std::vector<Widget*>& widgets, const char* labelText, TextLabelWidget*& outNumBox) {
    log(LOG_VERBOSE, "Inside WidgetLayoutHelper::initializeTestScreenLayout()");

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
