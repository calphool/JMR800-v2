#pragma once

#include "screens/Screen.h"

class ModalDialog : public Screen {

private:
    int xoffset;
    int yoffset;
    int width;
    int height;
    bool done;

public:
  ModalDialog(int x, int y, int w, int h);
  ~ModalDialog();
  virtual void draw() = 0;
  virtual bool isDone() const = 0;
  virtual void onEnter() = 0;
  virtual void onExit() = 0;
};