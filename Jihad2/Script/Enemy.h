#pragma once
#include "Collider.h"
#include "Wall.h"


class Enemy {
protected:
	Wall& wall;
	Collider& ground;

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
	
	bool onGround();

public:
	Enemy(int speed_, Wall& wall, Collider& ground);
	virtual ~Enemy() = default;

	virtual void move() = 0;
	virtual void climb() = 0;
	//virtual void attack() = 0;
	virtual void update() = 0;
	virtual void draw() const = 0;
	bool isDead() const { return dead; }

};


class Thumb : public Enemy {
private:
	enum class State { 
		Normal, Assimilating, Climbing, Attacking
	} state;
	static const int size;
	Point touched_wall_pos;

	void move() override;
	void asslimilate();
	void climb() override;
	//void attack();
	void changeState(const State& new_state) { state = new_state; t = 0; }

public:
	Thumb(Wall& wall, Collider& ground);
	~Thumb() = default;

	void update() override;
	void draw() const override;
};