#include "screens/ModalDialog.h"
#include "widgets/RectangleWidget.h"


ModalDialog::ModalDialog(int x, int y, int w, int h) : xoffset(x), yoffset(y), width(w), height(h) {
    done = false;
}

ModalDialog::~ModalDialog() {

}

void ModalDialog::onEnter() {
}

void ModalDialog::onExit() {

}

void ModalDialog::draw() {
   
}

bool ModalDialog::isDone()const  {
    return done;
}