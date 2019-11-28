#include <iostream>
using namespace std;
template<int x> void foo1 (void) {
	cout << x << endl;
}
/*
template<double x> void foo2 (void) {
	cout << x << endl;
}*/
/*
template<string x> void foo3 (void) {
	cout << x << endl;
}*/
template<char const* x> void foo4 (
	void) {
	cout << x << endl;
}
// 具有外部链接特性的全局字符数组
char g_str[] = "hello";
char const* g_ptr = "hello";
static char s_str[] = "hello";
int main (void) {
	foo1<3> ();
//	foo2<3.14> ();
//	foo4<"hello"> ();
	foo4<g_str> ();
//	foo4<g_ptr> ();
//	foo4<s_str> ();
	return 0;
}
