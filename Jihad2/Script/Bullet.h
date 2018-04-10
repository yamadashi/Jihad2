#pragma once
#include "Collider.h"
#include "Enemy.h"
#include "EventSystem.h"



using namespace std;


class Explosion : ymds::Event {
private:
	int t;

public:
	Explosion();
	void update() override;
	void draw() const override;
};



struct PointF {
	PointF(float x_, float y_) : x(x_), y(y_) {}
	PointF(const Point& point) : x((float)point.x), y((float)point.y) {}
	float x;
	float y;
};

class Bullet {
private:
	
protected:
	Point pos;
	PointF posF;
	float v_x, v_y;
	const float g;
	bool dead;
	static const int size;

	Collider collider;
	list<unique_ptr<Enemy>>& enemies;
	Wall& wall;
	Collider& ground;

	void move();
	bool intersects() const;
	bool isOutOfView() const;
	void onTouch();

public:
	Bullet(const Point& pos_, float speed, float angle, list<unique_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_);
	virtual ~Bullet() = default;
	virtual void update() = 0;
	virtual void draw() const = 0;
	bool isDead() const { return dead; }
	vector<Point> getTrajectory() const;
};


class Eye final : public Bullet {
private:


public:
	Eye(const Point& pos_, float speed, float angle, list<unique_ptr<Enemy>>& enemies_, Wall&, Collider& ground_);
	void update() override;
	void draw() const override;
};

class Sputum final : public Bullet {


};