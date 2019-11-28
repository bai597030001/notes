#include <iostream>
using namespace std;
#define MAX(T) \
T max_##T (T x, T y) { \
	return x < y ? y : x; \
}
MAX (int)
/*
int max_int (int x, int y) {
	return x < y ? y : x;
}
*/
MAX (double)
/*
double max_double (double x,
	double y) {
	return x < y ? y : x;
}
*/
MAX (string)
/*
string max_string (string x,
	string y) {
	return x < y ? y : x;
}
*/
#define max(T) max_##T
int main (void) {
	int nx = 123, ny = 456;
	cout << max(int) (nx, ny)
		<< endl;
	double dx = 1.23, dy = 4.56;
	cout << max(double) (dx, dy)
		<< endl;
	char sx[] = "hello",
		sy[] = "world";
	cout << max(string) (sx, sy)
		<< endl;
	return 0;
}
