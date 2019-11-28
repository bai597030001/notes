#include <iostream>
using namespace std;
template<typename T, size_t S = 3>
class Array {
public:
	T& operator[] (size_t i) {
		return m_array[i];
	}
	T const& operator[] (
		size_t i) const {
		return const_cast<Array&> (
			*this) [i];
	}
	size_t size (void) const {
		return S;
	}
	friend ostream& operator<< (
		ostream& os,
		Array<T, S> const& array) {
		for (size_t i = 0; 
			i < array.size (); ++i)
			os << '[' << array[i]
				<< ']';
		return os;
	}
private:
	T m_array[S];
};
template<int x> void foo (void) {
	cout << x << endl;
}
int bar (void) {
	return 100;
}
int main (void) {
	Array<int> ai;
	ai[0] = 100;
	ai[1] = 200;
	ai[2] = 300;
	cout << ai << endl;
	Array<string> as;
	as[0] = "北京";
	as[1] = "天津";
	as[2] = "上海";
	cout << as << endl;
	Array<Array<int> > aa;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			aa[i][j] = (i+1)*10+j+1;
// aa.operator[](i).operator[](j)=...
	cout << aa << endl;
	Array<Array<Array<int> > > aaa;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 3;
				++k)
				aaa[i][j][k] =
					(i+1)*100+
					(j+1)*10+
					(k+1);
	cout << aaa << endl;
//	Array<double, 5> ad;
//	Array<double, 2+3> ad;
	size_t const /*volatile*/ S = 5;
	Array<double, S> ad;
	ad[0] = 1.1;
	ad[1] = 2.2;
	ad[2] = 3.3;
	ad[3] = 4.4;
	ad[4] = 5.5;
	cout << ad << endl;
	foo<100> ();
//	foo<bar ()> ();
	int x = 4;
	foo<sizeof (x)> ();
//	foo<x> ();
	return 0;
}
