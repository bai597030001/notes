// 双向线性链表容器、迭代器及泛型算法
#include <cstring>
#include <iostream>
#include <stdexcept>
using namespace std;
// 双向线性链表类模板容器
template<typename T>
class List {
public:
	// 构造、析构、拷贝构造、拷贝赋值
	List (void) : m_head (NULL),
		m_tail (NULL) {}
	~List (void) {
		clear ();
	}
	List (List const& that) :
		m_head (NULL),
		m_tail (NULL) {
		for (Node* node =
			that.m_head; node;
			node = node->m_next)
			push_back (node->m_data);
	}
	List& operator= (
		List const& rhs) {
		if (&rhs != this) {
			List list = rhs;
			swap (m_head,
				list.m_head);
			swap (m_tail,
				list.m_tail);
		}
		return *this;
	}
	// 获取首元素
	T& front (void) {
		if (empty ())
			throw underflow_error (
				"链表下溢！");
		return m_head->m_data;
	}
	T const& front (void) const {
		return const_cast<List*> (
			this)->front ();
	}
	// 向首部压入
	void push_front (T const& data) {
		m_head = new Node (data,
			NULL, m_head);
		if (m_head->m_next)
			m_head->m_next->m_prev =
				m_head;
		else
			m_tail = m_head;
	}
	// 从首部弹出
	void pop_front (void) {
		if (empty ())
			throw underflow_error (
				"链表下溢！");
		Node* next = m_head->m_next;
		delete m_head;
		m_head = next;
		if (m_head)
			m_head->m_prev = NULL;
		else
			m_tail = NULL;
	}
	// 获取尾元素
	T& back (void) {
		if (empty ())
			throw underflow_error (
				"链表下溢！");
		return m_tail->m_data;
	}
	T const& back (void) const {
		return const_cast<List*> (
			this)->back ();
	}
	// 向尾部压入
	void push_back (T const& data) {
		m_tail = new Node (data,
			m_tail);
		if (m_tail->m_prev)
			m_tail->m_prev->m_next =
				m_tail;
		else
			m_head = m_tail;
	}
	// 从尾部弹出
	void pop_back (void) {
		if (empty ())
			throw underflow_error (
				"链表下溢！");
		Node* prev = m_tail->m_prev;
		delete m_tail;
		m_tail = prev;
		if (m_tail)
			m_tail->m_next = NULL;
		else
			m_head = NULL;
	}
	// 删除所有匹配元素
	void remove (T const& data) {
		for (Node* node = m_head,
			*next; node;
			node = next) {
			next = node->m_next;
			if (equal (node->m_data,
				data)) {
				if (node->m_prev)
					node->m_prev->
					m_next =
						node->m_next;
				else
					m_head =
						node->m_next;
				if (node->m_next)
					node->m_next->
					m_prev =
						node->m_prev;
				else
					m_tail =
						node->m_prev;
				delete node;
			}
		}
	}
	// 清空
	void clear (void) {
		while (! empty ())
			pop_back ();
	}
	// 判空
	bool empty (void) const {
		return ! m_head && ! m_tail;
	}
	// 大小
	size_t size (void) const {
		size_t nodes = 0;
		for (Node* node = m_head;
			node;
			node = node->m_next)
			++nodes;
		return nodes;
	}
	// 随机访问
	T& operator[] (size_t i) {
		for (Node* node = m_head;
			node;
			node = node->m_next)
			if (i-- == 0)
				return node->m_data;
		throw out_of_range (
			"下标越界！");
	}
	T const& operator[] (
		size_t i) const {
		return const_cast<List&> (
			*this)[i];
	}
	// 插入输出流
	friend ostream& operator<< (
		ostream& os,
		List const& list) {
		for (Node* node =
			list.m_head; node;
			node = node->m_next)
			os << *node;
		return os;

	}
private:
	// 节点
	class Node {
	public:
		Node (T const& data,
			Node* prev = NULL,
			Node* next = NULL) :
			m_data (data),
			m_prev (prev),
			m_next (next) {}
		friend ostream& operator<< (
			ostream& os,
			Node const& node) {
			return os << '['
				<< node.m_data
				<< ']';
		}
		T     m_data; // 数据
		Node* m_prev; // 前指针
		Node* m_next; // 后指针
	};
	// 判等函数的通用版本
	bool equal (T const& a,
		T const& b) const {
		return a == b;
	}
	Node* m_head; // 头指针
	Node* m_tail; // 尾指针
public:
	// 正向迭代器
	class Iterator {
	public:
		Iterator (Node* head = NULL,
			Node* tail = NULL,
			Node* node = NULL) :
			m_head (head),
			m_tail (tail),
			m_node (node) {}
		bool operator== (Iterator
			const& rhs) const {
			return m_node ==
				rhs.m_node;
		}
		bool operator!= (Iterator
			const& rhs) const {
			return ! (*this == rhs);
		}
		Iterator& operator++ (void) {
			if (m_node)
				m_node =
					m_node->m_next;
			else
				m_node = m_head;
			return *this;
		}
		Iterator const operator++ (
			int) {
			Iterator old = *this;
			++*this;
			return old;
		}
		Iterator& operator-- (void) {
			if (m_node)
				m_node =
					m_node->m_prev;
			else
				m_node = m_tail;
			return *this;
		}
		Iterator const operator-- (
			int) {
			Iterator old = *this;
			--*this;
			return old;
		}
		T& operator* (void) const {
			return m_node->m_data;
		}
		T* operator-> (void) const {
			return &**this;
		}
	private:
		Node* m_head;
		Node* m_tail;
		Node* m_node;
		friend class List;
	};
	// 获取起始正向迭代器
	// 指向容器的首元素
	Iterator begin (void) {
		return Iterator (m_head,
			m_tail, m_head);
	}
	// 获取终止正向迭代器
	// 指向容器的尾元素之后
	Iterator end (void) {
		return Iterator (m_head,
			m_tail);
	}
	// 在正向迭代器之前插入
	Iterator insert (Iterator loc,
		T const& data) {
		if (loc == end ()) {
			push_back (data);
			return Iterator (m_head,
				m_tail, m_tail);
		}
		else {
			Node* node = new Node (
				data,
				loc.m_node->m_prev,
				loc.m_node);
			if (node->m_prev)
				node->m_prev->m_next=
					node;
			else
				m_head = node;
			node->m_next->m_prev =
				node;
			return Iterator (m_head,
				m_tail, node);
		}
	}
	// 删除正向迭代器的目标元素
	Iterator erase (Iterator loc) {
		if (loc == end ())
			throw invalid_argument (
				"无效参数！");
		if (loc.m_node->m_prev)
			loc.m_node->m_prev->
			m_next =
				loc.m_node->m_next;
		else
			m_head =
				loc.m_node->m_next;
		if (loc.m_node->m_next)
			loc.m_node->m_next->
			m_prev =
				loc.m_node->m_prev;
		else
			m_tail =
				loc.m_node->m_prev;
		Node* next =
			loc.m_node->m_next;
		delete loc.m_node;
		return Iterator (m_head,
			m_tail, next);
	}
	// 常正向迭代器
	class ConstIterator {
	public:
		ConstIterator (void) {}
		ConstIterator (
			Iterator const& that) :
			m_it (that) {}
		bool operator== (
			ConstIterator const&
			rhs) const {
			return m_it == rhs.m_it;
		}
		bool operator!= (
			ConstIterator const&
			rhs) const {
			return ! (*this == rhs);
		}
		ConstIterator& operator++ (
			void) {
			++m_it;
			return *this;
		}
		ConstIterator const
			operator++ (int) {
			ConstIterator old =
				*this;
			++*this;
			return old;
		}
		ConstIterator& operator-- (
			void) {
			--m_it;
			return *this;
		}
		ConstIterator const
			operator-- (int) {
			ConstIterator old =
				*this;
			--*this;
			return old;
		}
		T const& operator* (
			void) const {
			return *m_it;
		}
		T const* operator-> (
			void) const {
			return &**this;
		}
	private:
		Iterator m_it;
	};
	// 获取起始常正向迭代器
	ConstIterator begin (void) const{
		return const_cast<List*> (
			this)->begin ();
	}
	// 获取终止常正向迭代器
	ConstIterator end (void) const {
		return const_cast<List*> (
			this)->end ();
	}
	// 反向迭代器
	// 常反向迭代器
};
// 判等函数针对char const*类型的特化
// 版本
template<>
bool List<char const*>::equal (
	char const* const& a,
	char const* const& b) const {
	return ! strcmp (a, b);
}
/*
int find (int a[], int size,
	int key) {
	for (int i = 0; i < size; ++i)
		if (a[i] == key)
			return i;
	return -1;
}
int* find (int* begin, int* end,
	int key) {
	int* it;
	for (it = begin; it != end &&
		*it != key; ++it);
	return it;
}
*/
// 泛型算法
template<typename IT, typename T>
IT find (IT begin, IT end, T key) {
	IT it;
	for (it = begin; it != end &&
		*it != key; ++it);
	return it;
}
template<typename IT>
void sort (IT begin, IT end) {
	IT p = begin;
	IT last = end;
	--last;
	for (IT i = begin, j = last;
		i != j;) {
		while (! (i == p || *p < *i))
			++i;
		if (i != p) {
			swap (*i, *p);
			p = i;
		}
		while (! (j == p || *j < *p))
			--j;
		if (j != p) {
			swap (*j, *p);
			p = j;
		}
	}
	IT it = begin;
	++it;
	if (p != begin && p != it)
		sort (begin, p);
	it = p;
	++it;
	if (it != end && it != last)
		sort (it, end);
}
template<typename IT, typename CMP>
void sort (IT begin, IT end,
	CMP cmp) {
	IT p = begin;
	IT last = end;
	--last;
	for (IT i = begin, j = last;
		i != j;) {
		while (! (i == p ||
			cmp (*p, *i)))
			++i;
		if (i != p) {
			swap (*i, *p);
			p = i;
		}
		while (! (j == p ||
			cmp (*j, *p)))
			--j;
		if (j != p) {
			swap (*j, *p);
			p = j;
		}
	}
	IT it = begin;
	++it;
	if (p != begin && p != it)
		sort (begin, p, cmp);
	it = p;
	++it;
	if (it != end && it != last)
		sort (it, end, cmp);
}
// 测试用例
void test1 (void) {
	List<int> l1;
	l1.push_front (50);
	l1.push_front (40);
	l1.push_front (30);
	l1.push_front (20);
	l1.push_front (10);
	cout << l1 << endl;
	// 10 20 30 40 50
	l1.pop_front ();
	cout << l1 << endl;
	// 20 30 40 50
	l1.front () = 25;
	cout << l1 << endl;
	// 25 30 40 50
	List<int> const& cr = l1;
//	++cr.front ();
	cout << cr.front () << endl;
	l1.push_back (60);
	l1.push_back (70);
	l1.push_back (80);
	l1.push_back (90);
	cout << l1 << endl;
	// 25 30 40 50 60 70 80 90
	l1.pop_back ();
	cout << l1 << endl;
	// 25 30 40 50 60 70 80
	l1.back () -= 5;
	cout << l1 << endl;
	// 25 30 40 50 60 70 75
	List<int> const* cp = &l1;
//	cp->back ()--;
	cout << cp->back () << endl;
	l1.push_front (50);
	l1.push_front (50);
	l1.push_back (50);
	l1.push_back (50);
	cout << l1 << endl;
	// 50 50 ... 50 ... 50 50
	l1.remove (50);
	cout << l1 << endl;
	// 25 30 40 60 70 75
	cout << l1.size () << ' '
		<< boolalpha << l1.empty ()
		<< endl;
	// 6 false
	l1.clear ();
	cout << l1.size () << ' '
		<< l1.empty () << endl;
	// 0 true
	l1.push_back (10);
	l1.push_back (20);
	l1.push_back (30);
	cout << l1 << endl;
	// 10 20 30
	List<int> l2 = l1; // 拷贝构造
	cout << l2 << endl;
	// 10 20 30
	++l1.front ();
	--l2.back ();
	cout << l1 << endl;
	// 11 20 30
	cout << l2 << endl;
	// 10 20 29
	l1 = l2; // 拷贝赋值
	cout << l1 << endl;
	// 10 20 29
	cout << l2 << endl;
	// 10 20 29
	l1.pop_front ();
	l2.pop_back ();
	cout << l1 << endl;
	// 20 29
	cout << l2 << endl;
	// 10 20
}
void test2 (void) {
	char sa[][256] = {"北京", "天津",
		"北京", "上海", "北京",
		"重庆"};
	List</*string*/char const*> ls;
	for (size_t i = 0; i < sizeof (
		sa) / sizeof (sa[0]); ++i)
		ls.push_back (sa[i]);
	cout << ls << endl;
	ls.remove ("北京");
	cout << ls << endl;
}
void test3 (void) {
	List<int> l1;
	for (int i = 0; i < 10; ++i)
		l1.push_back ((i+1)*10);
	cout << l1 << endl;
//	10 20 30 40 50 60 70 80 90 100
//	l1[4] /= 10;
	List<int>::Iterator it =
		l1.begin ();
	*(++++++++it) /= 10;
	cout << l1 << endl;
//	l1[4] = l1[1] + l1[8];
	List<int>::Iterator i1 = it,
		i2 = it;
	------i1;
	++++++++i2;
	*it = *i1 + *i2;
	cout << l1 << endl;
//	size_t size = l1.size ();
//	for (size_t i = 0; i < size; ++i)
//		l1[i] /= 10;
	for (it = l1.begin ();
		it != l1.end (); ++it)
		*it /= 10;
	cout << l1 << endl;
}
void test4 (void) {
	List<int> l1;
	l1.insert (l1.begin (), 10);
	l1.insert (l1.end (), 50);
	cout << l1 << endl;
	// 10 50
	List<int>::Iterator it =
		l1.begin ();
	it = l1.insert (
		l1.insert (
			l1.insert (++it, 40),
			30),
		20);
	cout << l1 << endl;
	// 10 20 30 40 50
	l1.erase (
		l1.erase (
			l1.erase (it)));
	cout << l1 << endl;
	// 10 50
	l1.erase (
		l1.erase (l1.begin ()));
	cout << l1 << endl;
}
void print (List<int> const& li) {
	for (List<int>::ConstIterator
		it = li.begin ();
		it != li.end (); ++it)
		cout << /*++*/*it << ' ';
	cout << endl;
}
void test5 (void) {
	List<int> l1;
	l1.push_back (100);
	l1.push_back (200);
	l1.push_back (300);
	/*
	cout << l1 << endl;
	List<int>::Iterator const it =
		l1.begin ();
	// 相当于指针常量(int *const)
	++*it;
//	cout << *++it << endl;
	cout << l1 << endl;
	*/
	print (l1);
}
void test6 (void) {
	int a[] = {13, 29, 17, 36, 25};
	/*
	int i = find (a, 5, 17);
	cout << i << endl;
	i = find (a, 5, 19);
	cout << i << endl;
	*/
	int* p = find (a, a + 5,
		/*17*/19);
	if (p == a + 5)
		cout << "没找到！" << endl;
	else
		cout << "找到了：" << *p
			<< endl;
	List<int> l1;
	for (int i = 0; i < 5; ++i)
		l1.push_back (a[i]);
	List<int>::Iterator it =
		find (l1.begin (),
			l1.end (), /*17*/19);
	if (it == l1.end ())
		cout << "没找到！" << endl;
	else
		cout << "找到了：" << *it
			<< endl;
}
bool cmpstr (char const* a,
	char const* b) {
	return strcmp (a, b) < 0;
}
class CmpStr {
public:
	CmpStr (bool raise = true) :
		m_raise (raise) {}
	bool operator() (char const* a,
		char const* b) {
		if (m_raise)
			return strcmp (a, b) < 0;
		else
			return strcmp (a, b) > 0;
	}
private:
	bool m_raise;
};
class Student {
public:
	Student (string const& name,
		int age) : m_name (name),
		m_age (age) {}
	/*
	bool operator< (Student const&
		rhs) const {
		if (m_name == rhs.m_name)
			return m_age < rhs.m_age;
		return m_name > rhs.m_name;
	}
	*/
	friend ostream& operator<< (
		ostream& os, Student const&
		student) {
		return os << student.m_name
			<< ' ' << student.m_age;
	}
private:
	string m_name;
	int m_age;
	friend class CmpStu;
};
class CmpStu {
public:
	bool operator() (
		Student const& x,
		Student const& y) const {
		if (x.m_name == y.m_name)
			return x.m_age < y.m_age;
		return x.m_name > y.m_name;
	}
};
void test7 (void) {
	char const* sa[] = {
		"beijing", "shanghai",
		"tianjin", "shanghai",
		"chongqing"};
	List<string> ls;
	for (size_t i = 0; i < sizeof (
		sa) / sizeof (sa[0]); ++i)
		ls.push_back (sa[i]);
	sort (ls.begin (), ls.end ());
	cout << ls << endl;
//	sort (&sa[0], &sa[5], cmpstr);
//	sort (&sa[0], &sa[5], CmpStr ());
	sort (&sa[0], &sa[5],
		CmpStr (false));
	for (size_t i = 0; i < sizeof (
		sa) / sizeof (sa[0]); ++i)
		cout << sa[i] << ' ';
	cout << endl;
	List<Student> lt;
	lt.push_back (Student (
		"AAA", 40));
	lt.push_back (Student (
		"DDD", 30));
	lt.push_back (Student (
		"AAA", 20));
	lt.push_back (Student (
		"CCC", 30));
	lt.push_back (Student (
		"DDD", 10));
//	sort (lt.begin (), lt.end ());
	sort (lt.begin (), lt.end (),
		CmpStu ());
	cout << lt << endl;
}
int main (void) {
//	test1 ();
//	test2 ();
//	test3 ();
//	test4 ();
//	test5 ();
//	test6 ();
	test7 ();
	return 0;
}
