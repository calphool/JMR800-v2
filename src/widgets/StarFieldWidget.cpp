
#include <cstdlib>
#include <cmath>
#include "widgets/StarFieldWidget.h"
#include "ScreenManager.h"
#include "Logging.h"

#ifdef TARGET_TEENSY
#define RANDOM_RNG(X, Y) random(X, Y)
#define RANDOM(X) random(X)
#else
#define RANDOM_RNG(X, Y) ((X) + (random() % ((Y) - (X))))
#define RANDOM(X) (random() % (X))
#endif

StarFieldWidget::StarFieldWidget(int x, int y, int _width, int _height) :
  Widget(x, y, false), width(_width), height(_height) {
  log(LOG_VERBOSE, "Inside StarFieldWidget->constructor", __func__);

  half_width = width / 2;
  half_height = height / 2;
  for (int i = 0; i < NUM_STARS; i++) {
    resetStar(stars[i]);
  }
}


void StarFieldWidget::resetStar(Star &s) {
  float angle = RANDOM_RNG(0, 6283) / 1000.0; // 0 to 2π
  float speed = 0.5 + (RANDOM(10) / 10.0); // 0.5 to 1.5

  s.dx = cos(angle) * speed;
  s.dy = sin(angle) * speed;
  s.x = x+(half_width);
  s.y = y+(half_height);
  s.speedMult = 1.01 + (RANDOM(5) / 100.0); // subtle growth per frame
}


void StarFieldWidget::draw() {
  ScreenManager::getDisplay()->fillRect(x, y, width, height, 0);
  
for (int i = 0; i < NUM_STARS; i++) {
    Star &s = stars[i];

    // Move outward
    s.x += s.dx;
    s.y += s.dy;

    // Apply acceleration-like scaling
    s.dx *= s.speedMult;
    s.dy *= s.speedMult;

    int px = (int)s.x;
    int py = (int)s.y;

    // If inside bounds, draw
    if (px >= x && px < x + width &&
        py >= y && py < y + height) {
      ScreenManager::getDisplay()->drawPixel(px, py, 1);
    } else {
      resetStar(s);
    }
  }
}


/**
 * @brief StarField doesn't take input
 */
void StarFieldWidget::handleInput() {
}

/**
 * @brief Returns the widget type as `WidgetType::StarField`.
 * @return WidgetType Enumeration value representing a Star Field
 */
WidgetType StarFieldWidget::getType() const {
    return WidgetType::StarField;
}