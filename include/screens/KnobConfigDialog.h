#pragma once
#include <vector>
#include "widgets/Widget.h"
#include "screens/ModalDialog.h"

class KnobConfigDialog : public ModalDialog {

private:
    std::vector<Widget*> widgets;
    int xoffset;
    int yoffset;
    int width;
    int height;
    bool done;
    int active_knob;

public:
    KnobConfigDialog(int x, int y, int w, int h, int knob);
    void draw() override;
    bool isDone() const;
    void onEnter() override;
    void onExit() override;
    void handleInput() override;
};
