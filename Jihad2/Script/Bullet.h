#pragma once
#include "Collider.h"
#include "Enemy.h"
#include "EventSystem.h"
#include "EffectData/EffectStrategy.h"



using namespace std;



class Bullet : public ymds::Event {
private:
	Point pos;
	PointF posF;
	Vec2 v;

protected:
	const float g;
	Collider collider;

	list<shared_ptr<Enemy>>& enemies;
	Wall& wall;
	Collider& ground;

	const Point& Pos() const { return pos; }
	const Vec2& Velocity() const { return v; }

	void move();
	void move(const Vec2& delta);
	bool isOutOfView() const;
	bool intersects() const;
	virtual void onTouch() = 0;

public:
	Bullet(const Point& pos_, const int size, const Vec2& v0, list<shared_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_);
	Bullet(const Point& pos_, const int size, float speed, float angle, list<shared_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_);
	virtual ~Bullet() = default;
	vector<Point> getTrajectory() const;
	virtual int Size() const = 0;
};


class Eye final : public Bullet {
private:
	static const float power; //‡yóÙéûÇÃê®Ç¢
	std::unique_ptr<AnimationGIFStrategy> explosion;
	bool exploding;
	void explode();

protected:
	void onTouch() override;

public:
	Eye(const Point& pos_, float speed, float angle, list<shared_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_);
	~Eye() = default;
	void update() override;
	void draw() const override;
	int Size() const { return size; }

	static const int size;
	static AnimationGIFData explosionGIF;
};


class TinyEye : public Bullet {
private:
	bool intersects();
	int count; //ê⁄êGâÒêî

protected:
	void onTouch() override;

public:
	TinyEye(const Point& pos_,const Vec2& v0, float speed, float agnle, list<shared_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_);
	~TinyEye() = default;
	void update() override;
	void draw() const override;
	int Size() const { return size; }

	static const int size;
};


class Sputum final : public Bullet {
private:
	enum class Intersection { OnLeft, Above, None } intersection;
	Intersection intersects();
	std::unique_ptr<LinkedImageStrategy> scatterAnimation;
	bool scattering;

protected:
	void onTouch() override;

public:
	Sputum(const Point& pos_, float speed, float angle, list<shared_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_);
	~Sputum() = default;
	void update() override;
	void draw() const override;
	int Size() const { return size; }

	static const int size;
};

class ScatteredSputum: public ymds::Event {
private:
	const Point pos;
	Collider collider;
	list<shared_ptr<Enemy>>& enemies;
	int count;
	const int limit;

public:
	ScatteredSputum(const Point& pos_, list<shared_ptr<Enemy>>& enemies_);
	~ScatteredSputum() = default;

	void update() override;
	void draw() const override;

};