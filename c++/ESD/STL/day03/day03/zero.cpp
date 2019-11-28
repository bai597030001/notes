#include <iostream>
using namespace std;
template<typename T>
void foo (void) {
//	T t;
//	string t;  // 缺省构造
//	int t;     // 定义的值
//	Point3D t; // 缺省构造
	T t = T (); // 零初始化
//	string t = string ();
//	int t = int ();
//	Point3D t = Point3D ();
	cout << t << endl;
}
class Point3D {
public:
	Point3D (int x = 0, int y = 0,
		int z = 0) : m_x (x),
		m_y (y), m_z (z) {}
	friend ostream& operator<< (
		ostream& os,
		Point3D const& pt) {
		return os << '(' << pt.m_x
			<< ',' << pt.m_y
			<< ',' << pt.m_z << ')';
	}
private:
	int m_x, m_y, m_z;
};
int main (void) {
	foo<string> ();
	foo<int> ();
	foo<Point3D> ();
	return 0;
}
