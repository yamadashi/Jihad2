#pragma once
#include "Collider.h"
#include "Wall.h"
#include "Damageable.h"
# include "ymdsUtility.h"

using namespace ymds;
using TouchedBlock = Optional<Point>;

class EnemyManager;

class Enemy : public Damageable {
public:
	enum class State {
		Forward, Back, Assimilating, Climbing, Attacking, Waiting, None
	};

protected:
	State state;
	Wall& wall;
	Collider& ground;
	std::pair<Collider&, Damageable&> fortress;

	EnemyManager& manager;
	Point pos;
	Collider collider;
	Collider bottom;
	int ground_y; //地面の位置
	bool dead;
	int h_speed, v_speed;
	static const int g;
	int t; //描画用変数
	int anim_coef; // アニメーション速度の係数
	
	bool isOnGround(); //地面に足がついているか
	bool isOnFloor(); //地面(階段含む)に足がついているか
	virtual TouchedBlock touchesOn() = 0;
	virtual void goForward() = 0;
	virtual void goBack() = 0;
	virtual void climb() = 0;
	virtual void attack() = 0;
	virtual void wait() = 0;

public:
	Enemy(int speed_, const Point& pos_, Wall& wall, Collider& ground, const std::pair<Collider&, Damageable&>& fortress, EnemyManager& manager_);
	virtual ~Enemy() = default;

	virtual void update() = 0;
	virtual void draw() const = 0;
	bool isDead() const { return dead; }
	bool isAssimilating() const { return state == State::Assimilating; }
	void kill() { dead = true; }
	void damage(int val) override { hp -= val; }
	const Collider& getCollider() const { return collider; }
	const State& getState() const { return state; }
	const Point& getPos() const { return pos; }
	void changeState(const State& new_state) { state = new_state; t = 0; }
};

class Thumb final : public Enemy {
private:
	static const int size;
	static const int climb_size;
	static const int collider_space_diff;
	TouchedBlock touchPos;
	bool hasClimbed;
	int climb_count;
	Point climbPos; //climb中の描画位置

protected:
	TouchedBlock touchesOn() override;
	void goForward() override;
	void goBack() override;
	void asslimilate();
	void climb() override;
	void attack() override;
	void wait() override;

public:
	Thumb(const Point& pos_, Wall& wall, Collider& ground, const std::pair<Collider&, Damageable&>& fortress, EnemyManager& manager_);
	~Thumb() = default;

	void update() override;
	void draw() const override;

	const TouchedBlock& getTouchPos() { return touchPos; }
	void setClimbed(void) { hasClimbed = true; }
	void transit();
};



class EnemyManager {
private:
	Wall& wall;
	Collider& ground;
	pair<Collider&, Damageable&> fortress;

	Stopwatch timer;

	list<shared_ptr<Enemy>> enemies;
	Point frontPos; //前線の位置
	int frontCol; //前線の列番号

	const int wait_time;

	void generate();

public:
	EnemyManager(Wall& wall_, Collider& ground_, std::pair<Collider&, Damageable&> fortress_);
	~EnemyManager() = default;
	void update();
	void draw() const;

	auto& getEnemies() { return enemies; }
	Point& getFrontPos() { return frontPos; }
	void setFrontPos(const Point& point) { frontPos.set(point); }
	int getFrontCol() { return frontCol; }
	void setFrontCol(int new_col) { frontCol = new_col; }

	Queue<Thumb*> wait_queue;
};
