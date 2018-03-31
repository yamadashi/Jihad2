#pragma once
#include "Bullet.h"
#include "Collider.h"


using namespace std;


class Fortress {
private:
	const Point pos;
	double angle;
	const double max_angle, min_angle;
	const Vec2 rotation_point;
	const double rotation_velo;

	list<shared_ptr<Bullet>> bullets;
	Collider collider;

	void rotate();
	void fire();

public:
	Fortress();
	~Fortress() { bullets.clear(); }

	void update();
	void draw();
	list<shared_ptr<Bullet>>& getBullets() { return bullets; }

};
