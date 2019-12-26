#include "sort.h"

bool compare(int x, int y) {
	int max, min;
	compare(x, y, y, &max, &min);
	return max == y;
}

inline void swap(int &a, int &b) {
	int t = a;
	a = b, b = t;
}

void bogo_sort(int n, int *a) {
	int rand = 23333;
	
	while (1) {
		int i;
		for (i = 1; i < n; i++) {
			if (!compare(a[i - 1], a[i])) break;
		}
		if (i == n) break;
		
		for (i = n - 1; i; i--) {
			rand = rand * 16807LL % 2147483647;
			swap(a[i], a[rand % (i + 1)]);
		}
	}
}

void sort(int n, int limit, int *a) {
	return bogo_sort(n, a);
}
