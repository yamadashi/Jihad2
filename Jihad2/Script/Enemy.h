#pragma once
#include "Collider.h"
#include "Wall.h"


class Enemy {
protected:
	Wall & wall;
	Point pos;
	Collider collider;
	bool dead;
	bool invincible;
	int h_speed;
	int v_speed;
	const int g;
	int t; //描画用変数
	int anim_coef; // アニメーション速度の係数

public:
	Enemy(int speed_, Wall& wall);
	virtual ~Enemy() = default;

	virtual void update() = 0;
	virtual void draw() const = 0;
	bool isDead() const { return dead; }

};


class Thumb : public Enemy {
private:
	enum class State { 
		Normal, Assimilating, Climbing, Attacking
	} state;
	const int size = 150;
	Point touched_wall_pos;

	void move();
	void asslimilate();
	void climb();
	//void attack();
	void changeState(const State& new_state) { state = new_state; t = 0; }

public:
	Thumb(Wall& wall);
	~Thumb() = default;

	void update() override;
	void draw() const override;
};