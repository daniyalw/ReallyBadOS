#pragma once

#include <stdio.h>
#include <nothing.h>

#define ASSERT(value) value ? donothing() : log::error("Assert failed!\n\nFile: %s\nLine: %d\nFunction: %s", __FILE__, __LINE__, __FUNCTION__);
