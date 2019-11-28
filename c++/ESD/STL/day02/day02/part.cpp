#include <iostream>
using namespace std;
template<typename A, typename B>
class X {
public:
	static void foo (void) {
		cout << "X<A,B>" << endl;
	}
};
template<typename A>
class X<A, short> {
public:
	static void foo (void) {
		cout << "X<A,short>" << endl;
	}
};
template<typename A>
class X<A, A> {
public:
	static void foo (void) {
		cout << "X<A,A>" << endl;
	}
};
template<typename A, typename B>
class X<A*, B*> {
public:
	static void foo (void) {
		cout << "X<A*,B*>" << endl;
	}
};
template<typename A>
class X<A*, A*> {
public:
	static void foo (void) {
		cout << "X<A*,A*>" << endl;
	}
};
template<typename A, typename B>
class X<A[], B[]> {
public:
	static void foo (void) {
		cout << "X<A[],B[]>" << endl;
	}
};
template<typename A, typename B>
void foo (void) {
	cout << "foo<A,B>" << endl;
}
/* 函数模板不可以局部特化
template<typename A>
void foo<A, short> (void) {
	cout << "foo<A,short>" << endl;
}
*/
// 函数模板可以完全特化
template<>
void foo<short, short> (void) {
	cout << "foo<short,short>"
		<< endl;
}
int main (void) {
	X<double, int>::foo ();
	X<int, short>::foo ();
	X<int, int>::foo ();
	X<int**, short****>::foo ();
	X<char*, char*>::foo ();
	X<int[], char[]>::foo ();
	foo<double, int> ();
	foo<short, short> ();
	return 0;
}
