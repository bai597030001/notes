#include <iostream>
using namespace std;
#define max(x,y) ((x)<(y)?(y):(x))
int main (void) {
	int nx = 123, ny = 456;
	cout << max (nx, ny) << endl;
	double dx = 1.23, dy = 4.56;
	cout << max (dx, dy) << endl;
	char sx[] = "hello",
		sy[] = "world";
	cout << max (sx, sy) << endl;
	return 0;
}
