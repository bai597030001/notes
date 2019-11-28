#include <cstdlib>
#include <iostream>
using namespace std;
template<typename T>
class A {
public:
	int m_var;
	void foo (void) {}
protected:
	class B {};
	void exit (int status) {
		cout << "再见！" << endl;
	}
};
template<typename T>
class C : public A<T> {
public:
	void bar (void) {
		/*
		A<T>::m_var = 10;
		A<T>::foo (); */
		this->m_var = 10;
		this->foo ();
		typename A<T>::B b;
		/*
		A<T>::exit (0); */
		this->exit (0);
	}
};
int main (void) {
	C<int> c;
	c.bar ();
	return 0;
}
