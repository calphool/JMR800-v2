#include "widgets/Widget.h"

Widget::Widget(int x, int y, bool highlighted)
    : x(x), y(y), bHighlighted(highlighted), toggle(false) {}


void Widget::setHighlighted(bool b) {
    bHighlighted = b;
}
