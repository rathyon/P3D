#include "Math.h"

float frand() {
	float num = (float) rand();
	return num / ((float)RAND_MAX);
}