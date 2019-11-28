#include <cstdlib>
#include <iostream>
#include <typeinfo>
using namespace std;
template<typename T>
void foo (T const& x, T const& y) {
	cout << typeid (x).name ()
		<< ' ' << typeid (y).name ()
		<< endl;
}
template<typename T>
void bar (T x, T y) {
	cout << typeid (x).name ()
		<< ' ' << typeid (y).name ()
		<< endl;
}
template<typename R, typename T>
//template<typename T, typename R>
R hum (T const& x) {
	R y;
	cout << typeid (x).name ()
		<< ' ' << typeid (y).name ()
		<< endl;
	return y;
}
int main (void) {
	int a, b;
	foo (a, b); // i i
	double c, d;
	foo (c, d); // d d
	char e[256], f[256];
	cout << sizeof (e) << endl;//整体
	e[1] = 'C'; // *(e+1) = 'C'//首址
	cout << &a << ' ' << (void*)a
		<< endl;
	cout << &e << ' ' << (void*)e
		<< endl; // 整体 vs. 首址
	foo (e, f); // A256_c A256_c 整体
	bar (e, f); // Pc Pc 首址
//	foo ("hello", "tarena");
//  T    A6_c     A7_c
	bar ("hello", "tarena");
//	T    PKc       PKc
	// 显式转换+隐式推断
	foo (a, static_cast<int> (c));
	// 显式实例化+隐式转换
	foo<double> (a, c);
	int i;
	double j;
//	i = hum (j); //返回类型不隐式推断
	i = hum<int, double> (j);
	i = hum<int> (j); // 半隐半显
	return 0;
}
