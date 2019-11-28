#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
void print (vector<int> const& vi) {
	cout << "字节：" << sizeof (vi)
		<< endl;
	size_t size = vi.size ();
	cout << "大小：" << size
		<< endl;
	cout << "元素：" << flush;
	for (size_t i = 0; i < size;
		++i)
		cout << vi[i] << ' ';
	cout << endl;
}
void show (vector<int> const& vi) {
	cout << "迭代：" << flush;
	for (vector<int>::const_iterator
		it = vi.begin (); it !=
		vi.end (); ++it)
		cout << *it << ' ';
	cout << endl;
}
void rshow (vector<int> const& vi) {
	cout << "反向：" << flush;
	for (vector<int>::
		const_reverse_iterator it =
		vi.rbegin (); it !=
		vi.rend (); ++it)
		cout << *it << ' ';
	cout << endl;
}
template<typename T>
void remove (vector<T>& vec,
	T const& key) {
	for (typename vector<T>::iterator
		it = vec.begin ();
		(it = find (it, vec.end (),
		key)) != vec.end ();
		it = vec.erase (it));
}
bool cmpint (int a, int b) {
	return a > b;
}
class CmpInt {
public:
	bool operator() (int a, int b)
		const {
		return a > b;
	}
};
int main (void) {
	vector<int> v1;
	print (v1);
	vector<int> v2 (5);
	print (v2);
	v2[0] = 10;
	v2[1] = 20;
	v2[2] = 30;
	v2[3] = 40;
	v2[4] = 50;
//	v2[5] = 60;
	v2.push_back (60);
	print (v2);
	vector<int> v3 (5, 13);
	print (v3);
	int a[10]={1,2,3,4,5,6,7,8,9,10};
	vector<int> v4 (a+3, a+7);
	print (v4);
	vector<int> v5 (&a[0], &a[10]);
	print (v5);
	show (v5);
	rshow (v5);
	vector<int>::iterator it1 =
		v5.begin ();
//	cout << *++++++++it1 << endl;
	cout << *(it1+4) << endl;
	it1 = v5.end ();
	cout << *(it1-6) << endl;
	it1 = v5.begin ();
	vector<int>::iterator it2 = it1;
	it2 += 4;
	cout << boolalpha;
	cout << (it1 < it2) << endl;
	cout << (it1 - it2) << endl;
	vector<int>::reverse_iterator
		it3 = v5.rbegin (), it4 =
		it3;
		it4 += 4;
	cout << (it3 < it4) << endl;
	cout << (it3 - it4) << endl;
	v5.front () = 0;
	print (v5);
	++v5.back ();
	print (v5);
	v5.erase (v5.begin ());
	print (v5);
	v5.pop_back ();
	print (v5);
	v5.insert (v5.begin (), 1);
	print (v5);
	v5.push_back (10);
	print (v5);
	cout << *v5.erase (v5.begin () +
		4) << endl;
	print (v5);
	cout << *v5.insert (v5.begin () +
		4, 5) << endl;
	print (v5);
	vector<int> v6;
	v6.push_back (13);
	v6.push_back (27);
	v6.push_back (13);
	v6.push_back (39);
	v6.push_back (13);
	print (v6);
	remove (v6, 13);
	print (v6);
	vector<string> v7;
	v7.push_back ("济南");
	v7.push_back ("淄博");
	v7.push_back ("济南");
	v7.push_back ("济南");
	v7.push_back ("潍坊");
	remove (v7, string ("济南"));
	for (vector<string>::iterator it
		= v7.begin (); it !=
		v7.end (); ++it)
		cout << *it << ' ';
	cout << endl;
	vector<int> v8;
	v8.push_back (28);
	v8.push_back (17);
	v8.push_back (23);
	v8.push_back (14);
	v8.push_back (39);
	v8.push_back (25);
	v8.push_back (46);
	v8.push_back (32);
	print (v8);
//	sort (v8.begin (), v8.end ());
//	sort (v8.rbegin (), v8.rend ());
	sort (v8.begin (), v8.end (),
		/*cmpint*/CmpInt ());
	print (v8);
	vector<int> v9;
	v9.push_back (100);
	it1 = v9.begin ();
	cout << *it1 << endl; // 100
	v9.push_back (200);
	it1 = v9.begin ();
	cout << *it1 << endl; // 100
	return 0;
}
