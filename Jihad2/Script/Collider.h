#pragma once
#include <Siv3D.hpp>

using namespace std;

class Collider {
private:
	vector<Rect> colliders;
	Point& parent_pos;
	Point pre_pos;

public:
	Collider(Point& parent_pos_) : parent_pos(parent_pos_), pre_pos(parent_pos) {}
	~Collider() { colliders.clear(); }

	bool intersects(const Collider& other) const;
	const vector<Rect>& get() const { return colliders; }
	void add(const Rect& arg) { colliders.emplace_back(arg); }
	void update();

};
