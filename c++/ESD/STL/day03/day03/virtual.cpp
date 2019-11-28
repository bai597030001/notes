#include <iostream>
using namespace std;
template<typename PEN>
class Rect {
public:
	virtual ~Rect (void) {}
	virtual void draw (
		PEN const& pen) const {
		cout << "绘制矩形" << endl;
	}
	template<typename BRUSH>
	virtual void draw (
		PEN const& pen,
		BRUSH const& brush) const {}
};
template<typename PEN>
class RoundRect : public Rect<PEN> {
public:
	void draw (
		PEN const& pen) const {
		cout << "绘制圆角矩形"
			<< endl;
	}
};
class SolidPen {};
int main (void) {
	SolidPen pen;
	Rect<SolidPen>* rect =
		new RoundRect<SolidPen>;
	rect->draw (pen);
	delete rect;
	return 0;
}
