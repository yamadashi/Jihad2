#pragma once
#include <Siv3D.hpp>

using namespace std;

class Collider {
private:
	Optional<Rect&> collider;
	vector<Rect> colliders;
	const Point& parent_pos;
	Point pre_pos;
	Point delta;

public:
	Collider(const Point& parent_pos_) : parent_pos(parent_pos_), pre_pos(parent_pos), delta(0, 0) {}
	~Collider() { colliders.clear(); }

	bool intersects(const Collider& other) const;
	const vector<Rect>& get() const { return colliders; }
	const Optional<Rect&>& getOne() const { return collider; } //コライダー一個のやつ用
	void add(const Rect& arg);
	void update();
};
