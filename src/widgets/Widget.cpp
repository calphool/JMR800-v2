#include "widgets/Widget.h"


/* -----------------------------------------------------------------
   |  Constructor                                                  |
   ----------------------------------------------------------------- */
Widget::Widget(int x, int y, bool highlighted)
    : x(x), y(y), bHighlighted(highlighted), toggle(false) {}

/* -----------------------------------------------------------------
   |  setHighlighted() - indicates whether a widget is highlighted |
   ----------------------------------------------------------------- */
void Widget::setHighlighted(bool b) {
    bHighlighted = b;
}
