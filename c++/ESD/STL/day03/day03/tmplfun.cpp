#include <iostream>
#include <typeinfo>
using namespace std;
template<typename T>
class Stack {
public:
	Stack (void) {}
	// 模板型模板成员函数
	template<typename E>
	Stack (Stack<E> const& that) {
		cout << "构造：Stack<"
			<< typeid (E).name ()
			<< "> -> Stack<"
			<< typeid (T).name ()
			<< ">" << endl;
	}
	template<typename E>
	Stack<T>& operator= (
		Stack<E> const& rhs);
};
template<typename T>
	template<typename E>
Stack<T>& Stack<T>::operator= (
	Stack<E> const& rhs) {
	cout << "赋值：Stack<"
		<< typeid (E).name ()
		<< "> -> Stack<"
		<< typeid (T).name ()
		<< ">" << endl;
}
int main (void) {
	Stack<double> sd;
	Stack<int> si = sd; // 构造
	sd = si; // 赋值
	return 0;
}
