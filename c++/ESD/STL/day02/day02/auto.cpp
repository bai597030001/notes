#include <iostream>
using namespace std;
template<typename T>
class AutoPtr {
public:
	explicit AutoPtr (T* p = NULL) :
		m_p (p) {}
	// 支持转移拷贝的拷贝构造
	AutoPtr (AutoPtr& that) :
		m_p (that.release ()) {}
	// 支持转移拷贝的拷贝赋值
	AutoPtr& operator= (
		AutoPtr& rhs) {
		if (&rhs != this)
			reset (rhs.release ());
		return *this;
	}
	~AutoPtr (void) {
		if (m_p) {
			delete m_p;
			m_p = NULL;
		}
	}
	T& operator* (void) const {
		return *m_p;
	}
	T* operator-> (void) const {
		return &**this;
	}
private:
	T* release (void) {
		T* p = m_p;
		m_p = NULL;
		return p;
	}
	void reset (T* p) {
		if (p != m_p) {
			delete m_p;
			m_p = p;
		}
	}
	T* m_p;
};
template<typename T>
class AutoPtr<T[]> {
public:
	explicit AutoPtr (T* p = NULL) :
		m_p (p) {}
	// 支持转移拷贝的拷贝构造
	AutoPtr (AutoPtr& that) :
		m_p (that.release ()) {}
	// 支持转移拷贝的拷贝赋值
	AutoPtr& operator= (
		AutoPtr& rhs) {
		if (&rhs != this)
			reset (rhs.release ());
		return *this;
	}
	~AutoPtr (void) {
		if (m_p) {
			delete[] m_p;
			m_p = NULL;
		}
	}
	T& operator* (void) const {
		return *m_p;
	}
	T* operator-> (void) const {
		return &**this;
	}
private:
	T* release (void) {
		T* p = m_p;
		m_p = NULL;
		return p;
	}
	void reset (T* p) {
		if (p != m_p) {
			delete[] m_p;
			m_p = p;
		}
	}
	T* m_p;
};
class A {
public:
	A (int data = 0) :
		m_data (data) {
		cout << "A构造：" << this
			<< endl;
	}
	~A (void) {
		cout << "A析构：" << this
			<< endl;
	}
	void inc (void) {
		++m_data;
	}
	int m_data;
};
void bar (AutoPtr<A> p) {}
void foo (void) {
	AutoPtr<A> p1 (new A (100));
	p1->inc ();
	cout << (*p1).m_data << endl;
	AutoPtr<A> p2 = p1; // 拷贝构造
	p2->inc ();
	cout << (*p2).m_data << endl;
	AutoPtr<A> p3 (new A (1000));
	p2 = p3; // 拷贝赋值
	p2->inc ();
	cout << (*p2).m_data << endl;
	/*
	bar (p2);
	p2->inc ();
	*/
}
int main (void) {
//	foo ();
	AutoPtr<A[]> p (new A[3]);
	return 0;
}
