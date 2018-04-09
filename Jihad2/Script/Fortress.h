#pragma once
#include "Bullet.h"
#include "Collider.h"
#include "Damageable.h"


using namespace std;


class Fortress : public Damageable {
private:
	const Point pos;
	double angle;
	const double angle_offset;
	const double max_angle, min_angle;
	const Vec2 rotation_point;
	const Vec2 muzzle_offset;
	const double rotation_velo;
	const int size;
	const float max_power;

	Collider collider;
	list<unique_ptr<Bullet>> bullets;
	list<unique_ptr<Enemy>>& enemies;
	Wall& wall;
	Collider& ground;

	void rotate();
	void fire();

public:
	Fortress(list<unique_ptr<Enemy>>&, Wall&, Collider& ground_);
	~Fortress() { bullets.clear(); }

	void update();
	void draw() const;
	list<unique_ptr<Bullet>>& getBullets() { return bullets; }
	Collider& getCollider() { return collider; }
	void damage(int val) override { hp -= val; }

};
