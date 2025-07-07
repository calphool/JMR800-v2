#pragma once

#include <memory>
#include "widgets/CenterWidget.h"

std::unique_ptr<CenterWidget> makeWidget(uint8_t typecode);
