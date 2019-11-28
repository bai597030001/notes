#include <iostream>
using namespace std;
class A {
public:
	// 内部类型
	typedef unsigned int UINT;
	typedef enum {
		RED, GEEN, BLUE } COLOR;
	class X {};
};
template<typename T>
void foo (void) {
	// 解决嵌套依赖
	typename T::UINT u;
	typename T::COLOR c;
	typename T::X x;
}
int main (void) {
	A::UINT u;
	A::COLOR c;
	A::X x;
	foo<A> ();
	return 0;
}
