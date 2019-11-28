#include <iostream>
using namespace std;
template<typename T>
class Array {
public:
	void push_back (T const& data) {
		cout << "向数组中压入"
			<< data << endl;
	}
};
template<typename T>
class List {
public:
	void push_back (T const& data) {
		cout << "向链表中压入"
			<< data << endl;
	}
};
template<typename T,
	// 模板型模板参数
	template<typename /*E*/>
	class C = List>
class Stack {
public:
	void push (T const& data) {
		m_cntr.push_back (data);
	}
private:
	C<T> m_cntr;
	// s1: Array<int>
	// s2: List<string>
};
int main (void) {
	Stack<int, Array> s1;
	s1.push (100);
	Stack<string/*, List*/> s2;
	s2.push ("北京");
	return 0;
}
