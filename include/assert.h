#pragma once

#include <stdio.h>
#include <nothing.h>

#define ASSERT(value) value ? donothing() : error("Assert failed!\n\nFile: %s\nLine: %d\nFunction: %s\n", __FILE__, __LINE__, __FUNCTION__);
