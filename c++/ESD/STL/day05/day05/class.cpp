#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
class A {
public:
	A (void) {
		cout << "缺省构造："
			<< this << endl;
	}
	A (A const& that) {
		cout << "拷贝构造："
			<< &that << "->" << this
			<< endl;
	}
	A& operator= (A const& rhs) {
		cout << "拷贝赋值："
			<< &rhs << "->" << this
			<< endl;
	}
	~A (void) {
		cout << "析构函数："
			<< this << endl;
	}
	bool operator== (A const& rhs)
		const { return true; }
	bool operator< (A const& rhs)
		const { return true; }
};
int main (void) {
	cout << "---- 1 ----" << endl;
	vector<A> v1 (3);
	cout << "---- 2 ----" << endl;
	v1.erase (v1.begin ());
	cout << "---- 3 ----" << endl;
	find (v1.begin (), v1.end (),
		A ());
	cout << "---- 4 ----" << endl;
	sort (v1.begin (), v1.end ());
	cout << "---- X ----" << endl;
	return 0;
}
