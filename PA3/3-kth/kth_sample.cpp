#include "kth.h"

void get_kth(int n, int k, int *x, int *y, int *z) {
	*x = *y = *z = 1;
	if (n > 1 && compare(1, 1, 1, 1, 2, 1)) {
		*y = 2;
	}
}
