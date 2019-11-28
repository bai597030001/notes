#include <iostream>
using namespace std;
// 类模板
template<typename T>
class Comparator {
public:
	Comparator (T x, T y) :
		m_x (x), m_y (y) {}
	T max (void) const {
		return m_x < m_y ? m_y : m_x;
	}
	T min (void) const {
		return m_x > m_y ? m_y : m_x;
	}
private:
	T m_x;
	T m_y;
};
class Integer {
public:
	Integer (int i = 0) : m_i (i) {}
	friend ostream& operator<< (
		ostream& os,
		Integer const& i) {
		return os << i.m_i;
	}
	bool operator< (
		Integer const& rhs) const {
		return m_i < rhs.m_i;
	}
private:
	int m_i;
};
int main (void) {
	Comparator<int> ci (123, 456);
	cout << ci.max () << ' '
		<< ci.min () << endl;
	Comparator<double> cd (1.23,
		4.56);
	cout << cd.max () << ' '
		<< cd.min () << endl;
	Comparator<string> cs ("hello",
		"world");
	cout << cs.max () << ' '
		<< cs.min () << endl;
	Comparator<Integer> cn (123,
		456);
	cout << cn.max () << endl;
	return 0;
}
