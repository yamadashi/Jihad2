#pragma once
#include "Collider.h"
#include "Enemy.h"
#include "EventSystem.h"
#include "EffectData/EffectStrategy.h"



using namespace std;


struct PointF {
	PointF(float x_, float y_) : x(x_), y(y_) {}
	PointF(const Point& point) : x((float)point.x), y((float)point.y) {}
	float x;
	float y;
	void moveBy(const Vec2& delta) { x += delta.x; y += delta.y; }
	const Vec2&& movedBy(const Vec2& delta) const { return Vec2(x + delta.x, y + delta.y); }
	const Vec2&& asVec2() { return Vec2(x, y); }
};


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
	static AnimationGIFData explosionGIF;
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
	static void setExplosionGIF(const FilePath& path) { explosionGIF.reset(path); }
	int Size() const { return size; }

	static const int size;
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
	bool intersects();
	enum class State { Injected, Spreading };

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