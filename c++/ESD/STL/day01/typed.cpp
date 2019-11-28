#include <iostream>
using namespace std;
int max_int (int x, int y) {
	return x < y ? y : x;
}
double max_double (double x,
	double y) {
	return x < y ? y : x;
}
string max_string (string x,
	string y) {
	return x < y ? y : x;
}
int main (void) {
	int nx = 123, ny = 456;
	cout << max_int (nx, ny) << endl;
	double dx = 1.23, dy = 4.56;
	cout << max_double (dx, dy)
		<< endl;
	char sx[] = "hello",
		sy[] = "world";
	cout << max_string (sx, sy)
		<< endl;
	return 0;
}
