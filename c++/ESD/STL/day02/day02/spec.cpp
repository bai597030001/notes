#include <iostream>
#include <cstring>
using namespace std;
template<typename T>
T max (T x, T y) {
	return x < y ? y : x;
}
char* max (char* x, char* y) {
	return strcmp (x, y) < 0 ? y : x;
}
// 通用版本(基本版本)
template<typename T>
class Comparator {
public:
	Comparator (T x, T y) :
		m_x (x), m_y (y) {}
	T max (void) const {
		return m_x < m_y ? m_y : m_x;
	}
//	char* max (void) const { ... }
private:
	T m_x;
	T m_y;
};
#if 0
// 针对char*类型的全类特化版本
template<>
class Comparator<char*> {
public:
	/*
	Comparator (char* x, char* y) :
		m_x (x), m_y (y) {} */
//	char* max (void) const {
	static char* zuidazhi (char* x,
		char* y) {
		return strcmp (x, y) < 0 ?
			y : x;
	}
/*
private:
	char* m_x;
	char* m_y; */
};
#endif
// 针对char*类型的成员特化版本
template<>
char* Comparator<char*>::max (
	void) const {
	return strcmp (m_x, m_y) < 0 ?
		m_y : m_x;
}
int main (void) {
	int a = 123, b = 456;
	cout << ::max (a, b) << endl;
	double c = 1.23, d = 4.56;
	cout << ::max (c, d) << endl;
	string e = "hello", f = "world";
	cout << ::max (e, f) << endl;
	char g[] = "hello",h[] = "world";
	cout << ::max (g, h) << endl;
//	cout << ::max (string (g),
//		string (h)) << endl;
//	cout << ::max<string> (g, h)
//		<< endl;
	cout << "----------------"
		<< endl;
	Comparator<int> ci (a, b);
	cout << ci.max () << endl;
	Comparator<double> cd (c, d);
	cout << cd.max () << endl;
	Comparator<string> cs (e, f);
	cout << cs.max () << endl;
//	Comparator<char*> cp/* (g, h)*/;
//	cout << cp.zuidazhi (g, h)
//		<< endl;
	Comparator<char*> cp (g, h);
	cout << cp.max () << endl;
	return 0;
}
