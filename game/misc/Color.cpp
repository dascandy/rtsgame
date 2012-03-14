#include "Color.h"

Color operator+(const Color &a, const Color &b) {
	return Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}
