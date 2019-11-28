#include <iostream>
#include <typeinfo>
using namespace std;
template<
	typename A = int,
	typename B = double,
	typename C = string>
class X {
public:
	static void print (void) {
		cout << typeid (A).name ()
			<< ' '
			<< typeid (B).name ()
			<< ' '
			<< typeid (C).name ()
			<< endl;
	}
};
template<
	typename A = int,
	typename B = double,
	typename C = string>
void print (void) {
	cout << typeid (A).name () << ' '
		<< typeid (B).name () << ' '
		<< typeid (C).name ()<< endl;
}
template<typename T = int>
void foo (T arg) {
	cout << typeid (T).name ()
		<< endl;
}
/*
void bar (int x, int y = x) {
	cout << x << ' ' << y << endl;
}
*/
template<typename A,
	typename B = A*>
//template<typename A = B,
//	typename B = int>
void hum (void) {
	cout << typeid (A).name ()
		<< ' ' << typeid (B).name ()
		<< endl;
}
int main (void) {
	X<short, float, char>::print ();
	X<short, float>::print ();
	X<short>::print ();
	X<>::print ();
//	X::print (); // 错误
	print<short, float, char> ();
	print<short, float> ();
	print<short> ();
	print<> ();
	foo (3.14);
//	bar (5);
	hum<double> ();
//	hum<> ();
	return 0;
}
