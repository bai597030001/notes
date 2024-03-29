#include "print.h"
#include <list>
#include <algorithm>
int main (void) {
	int a[] = {10,10,20,20,10,20,30,
		20,20,10,10};
	list<int> li (a, a + sizeof (a) /
		sizeof (a[0]));
	print (li.begin (), li.end ());
//	sort (li.begin (), li.end ());
	li.sort ();
	print (li.begin (), li.end ());
	li.unique ();
	print (li.begin (), li.end ());
	list<int> l2;
	l2.push_back (5000);
	l2.push_back (6000);
	l2.push_back (7000);
	l2.push_back (8000);
	l2.push_back (9000);
	print (l2.begin (), l2.end ());
	list<int>::iterator pos =
		li.begin ();
//	li.splice (++pos, l2);
//	list<int>::iterator del =
//		l2.begin ();
//	li.splice (++pos, l2, ++++del);
	list<int>::iterator begin =
		l2.begin(), end = l2.end ();
	li.splice (++pos, l2, ++begin,
		--end);
	print (li.begin (), li.end ());
	print (l2.begin (), l2.end ());
	list<int> la;
	la.push_back (19);
	la.push_back (23);
	la.push_back (36);
	la.push_back (44);
	la.push_back (58);
	list<int> lb;
	lb.push_back (11);
	lb.push_back (27);
	lb.push_back (31);
	lb.push_back (38);
	print (la.begin (), la.end ());
	print (lb.begin (), lb.end ());
	la.merge (lb);
	print (la.begin (), la.end ());
	print (lb.begin (), lb.end ());
	return 0;
}
