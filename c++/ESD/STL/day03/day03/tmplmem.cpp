#include <iostream>
using namespace std;
template<typename T> class List {
public:
	void push_back (T const& elem) {
		cout << "在链表尾端压入"
			<< elem << endl;
	}
	void pop_back (void) {
		cout << "从链表尾端弹出"
			<< endl;
	}
};
template<typename T> class Stack {
public:
	void push (T const& elem) {
		m_list.push_back (elem);
	}
	void pop (void) {
		m_list.pop_back ();
	}
private:
	List<T> m_list;
	// 模板型模板成员变量
};
int main (void) {
	Stack<int> si;
	si.push (10);
	si.pop ();
	return 0;
}
