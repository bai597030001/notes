#include <iostream>
#include <map>
using namespace std;
int main (void) {
	multimap<string, int> msi;
	msi.insert (
		make_pair ("张飞", 10000));
	msi.insert (
		make_pair ("赵云", 20000));
	msi.insert (
		make_pair ("关羽", 30000));
	msi.insert (
		make_pair ("张飞", 40000));
	msi.insert (
		make_pair ("赵云", 50000));
	msi.insert (
		make_pair ("关羽", 60000));
	typedef multimap<string, int>::
		iterator IT;
	for (IT it = msi.begin (); it !=
		msi.end (); ++it)
		cout << it->first << "："
			<< it->second << endl;
	cout << "----------" << endl;
	/*
	IT lower = msi.lower_bound (
		"张飞");
	IT upper = msi.upper_bound (
		"张飞");
	*/
	pair<IT, IT> res =
		msi.equal_range ("张飞");
	IT lower = res.first;
	IT upper = res.second;
	cout << lower->first << "："
		<< lower->second << endl;
	cout << upper->first << "："
		<< upper->second << endl;
	cout << "----------" << endl;
	for (IT it = lower; it != upper;
		++it)
		cout << it->first << "："
			<< it->second << endl;
	return 0;
}
