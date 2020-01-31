#include "MathFunc.h"

float lerp(float start, float end, float t) {
    return (1 - t) * start + t * end;
}
