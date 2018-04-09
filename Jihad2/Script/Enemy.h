#pragma once
#include "Collider.h"
#include "Wall.h"
#include "Damageable.h"


class Enemy : public Damageable {
protected:
	Wall& wall;
	Collider& ground;
	std::pair<Collider&, Damageable&> fortress;

	Point pos;
	Collider collider;
	Collider bottom;
	int ground_y;
	bool dead;
	bool invincible;
	int h_speed;
	int v_speed;
	const int g;
	int t; //描画用変数
	int anim_coef; // アニメーション速度の係数
	
	bool isOnGround();
	virtual void onTouch() = 0;
	virtual void move() = 0;
	virtual void climb() = 0;
	virtual void attack() = 0;
	virtual void wait() = 0;

public:
	Enemy(int speed_, Wall& wall, Collider& ground, const std::pair<Collider&, Damageable&>& fortress);
	virtual ~Enemy() = default;

	virtual void update() = 0;
	virtual void draw() const = 0;
	bool isDead() const { return dead; }
	void damage(int val) override { hp -= val; }
	const Collider& getCollider() const { return collider; }
};


class Thumb final : public Enemy {
private:
	enum class State { 
		Moving, Assimilating, Climbing, Attacking, Waiting
	} state;
	static const int size;
	static const int climb_size;
	static const int collider_space_diff;
	Point touched_wall_pos;

protected:
	void onTouch() override;
	void move() override;
	void asslimilate();
	void climb() override;
	void attack() override;
	void wait() override;
	void changeState(const State& new_state) { state = new_state; t = 0; }

public:
	Thumb(Wall& wall, Collider& ground, const std::pair<Collider&, Damageable&>& fortress);
	~Thumb() = default;

	void update() override;
	void draw() const override;
};