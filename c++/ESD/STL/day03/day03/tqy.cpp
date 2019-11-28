#include <iostream>
using namespace std;
template<typename T, size_t S>
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
	/*
	template<typename U>
	friend ostream& operator<< (
		ostream& os, U const& array){
	*//*
	friend ostream& operator<< (
		ostream& os,
		Array const& array) {
	*/
	template<typename U>
	friend ostream& operator+ (
		ostream& os, U const& array){
		for (size_t i = 0; i < S;
			++i)
			os << '[' << array[i]
				<< ']';
		return os;
	}
private:
	T m_array[S];
};
int main (void) {
	Array<int, 3> a;
	a[0] = 100;
	a[1] = 200;
	a[2] = 300;
	cout + a << endl;
	return 0;
}
