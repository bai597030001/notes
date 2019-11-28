#include <cstdlib>
#include <cstring>
#include <iostream>
#include <typeinfo>
using namespace std;
// 两个任意类型对象的最大值
template<typename T>
T const& max (T const& x,
	T const& y) {
	cout << "<1"
		<< typeid (x).name () << '>'
		<< endl;
	return x < y ? y : x;
}
// 两个任意类型指针所指向的目标对象
// 的最大值
template<typename T>
T* const& max (T* const& x,
	T* const& y) {
	cout << "<2"
		<< typeid (x).name () << '>'
		<< endl;
	return *x < *y ? y : x;
}
// 两个C风格字符串的最大值
char const* const& max (
	char const* const& x,
	char const* const& y) {
	cout << "<3"
		<< typeid (x).name () << '>'
		<< endl;
	return strcmp (x,y) < 0 ? y : x;
}
/* 会在4版本中返回临时变量的引用
   在重载函数模板时，其参数和返回类型
   要么都用引用，要么都不用引用，最好
   不要混合使用，以避免出现无法意料的
   问题
char const* max (char const* x,
	char const* y) {
	return strcmp (x,y) < 0 ? y : x;
}
*/
// 三个任意类型对象的最大值
template<typename T>
T const& max (T const& x,
	T const& y, T const& z) {
	cout << "<4"
		<< typeid (x).name () << '>'
		<< endl;
	return ::max (::max (x, y), z);
}
/*
// 两个C风格字符串的最大值
char const* const& max (
	char const* const& x,
	char const* const& y) {
	cout << "<3"
		<< typeid (x).name () << '>'
		<< endl;
	return strcmp (x,y) < 0 ? y : x;
}
*/
int main (void) {
	char const* x = "abc";
	char const* y = "ab";
	char const* z = "a";
	// 编译器优先选择普通函数
	cout << ::max (x, y) << endl; //3
	// 除非模板函数能够产生更好的匹配
	cout << ::max (100, 200) << endl;
	// 通过模板参数表告知编译器使用函
	// 数模板，类型针对性强的版本优先
	cout << ::max<> (y, x) << endl;
	// 显式指定的模板参数必须在所选择
	// 的重载版本中与调用参数的类型保
	// 持一致
	cout << ::max<char const*> (y, x)
		<< endl;
	cout << ::max (x, y, z) << endl;
	char const* const& r = ::max (
		x, y, z);
	cout << r << endl; // abc
	char const* a = "123";
	char const* b = "12";
	char const* c = "1";
	::max (a, b, c);
	cout << r << endl; // abc
	return 0;
}
