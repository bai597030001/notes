#include <iostream>
using namespace std;
// 函数模板
template<typename T>
T max (T x, T y) {
	return x < y ? y : x;
//	return x.operator< (y) ? y : x;
}
/*
int max (int x, int y) {
	return x < y ? y : x;
}
double max (double x, double y) {
	return x < y ? y : x;
}
string max (string x, string y) {
	return x < y ? y : x;
}
*/
class Integer {
public:
	Integer (int i) : m_i (i) {}
	bool operator< (
		Integer const& rhs) const {
		return m_i < rhs.m_i;
	}
	int m_i;
};
int main (void) {
	int nx = 123, ny = 456;
//	cout << ::max<int> (nx, ny)
//		<< endl;
	cout << ::max (nx, ny) << endl;
	double dx = 1.23, dy = 4.56;
//	cout << ::max<double> (dx, dy)
//		<< endl;
	cout << ::max (dx, dy) << endl;
	char sx[] = "hello",
		sy[] = "world";
	cout << ::max<string> (sx, sy)
		<< endl;
//	cout << ::max (sx, sy) << endl;
	Integer ix (123), iy (456);
//	cout << ::max<Integer> (ix, iy).
//		m_i << endl;
	cout << ::max (ix, iy).m_i
		<< endl;
	return 0;
}
