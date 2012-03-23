#include "Coherent.h"

double Coherent::getInputValue(int x, int y) {
	return (double)((unsigned int)(x*x*seed1 + y*y*seed2 + x*y*seed1*seed2)) / 4294967296.0;
}

double interp(double t, double P0, double P1, double P2, double P3) {
	double t2 = t*t;
	double t3 = t2 * t;
	return 0.5 *((2 * P1) +
	  		(-P0 + P2) * t +
			(2*P0 - 5*P1 + 4*P2 - P3) * t2 +
			(-P0 + 3*P1- 3*P2 + P3) * t3);
}

double Coherent::getLayerValue(double x, double y) {
	int bx = (int)x - 1;
	int by = (int)y - 1;
	double v11 = getInputValue(bx, by),
		   v12 = getInputValue(bx+1, by),
		   v13 = getInputValue(bx+2, by),
		   v14 = getInputValue(bx+3, by),
		   v21 = getInputValue(bx, by+1),
		   v22 = getInputValue(bx+1, by+1),
		   v23 = getInputValue(bx+2, by+1),
		   v24 = getInputValue(bx+3, by+1),
		   v31 = getInputValue(bx, by+2),
		   v32 = getInputValue(bx+1, by+2),
		   v33 = getInputValue(bx+2, by+2),
		   v34 = getInputValue(bx+3, by+2),
		   v41 = getInputValue(bx, by+3),
		   v42 = getInputValue(bx+1, by+3),
		   v43 = getInputValue(bx+2, by+3),
		   v44 = getInputValue(bx+3, by+3);
	double v1 = interp(x - (int)x, v11, v12, v13, v14);
	double v2 = interp(x - (int)x, v21, v22, v23, v24);
	double v3 = interp(x - (int)x, v31, v32, v33, v34);
	double v4 = interp(x - (int)x, v41, v42, v43, v44);
	return interp(y - (int)y, v1, v2, v3, v4);
}

double Coherent::getValue(double x, double y) {
	double total = 0, factor = 1.0 / (1 << levels);
	double scale = 2000.0 / (1 << levels);
	for (int i = 0; i < levels; i++) {
		total += getLayerValue(x / scale, y / scale) * factor;
		factor *= 2;
		scale *= 2;
	}
	return total;
}

Coherent::Coherent(int levels, unsigned int seed1, unsigned int seed2) 
: seed1(seed1)
, seed2(seed2)
, levels(levels)
{
}

