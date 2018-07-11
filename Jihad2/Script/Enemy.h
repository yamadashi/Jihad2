#pragma once
#include "Collider.h"
#include "Wall.h"
#include "Damageable.h"
# include "ymdsUtility.h"

using namespace ymds;
using TouchedBlock = Optional<Point>;

class EnemyManager;


struct PointF {
	PointF(float x_, float y_) : x(x_), y(y_) {}
	PointF(const Point& point) : x((float)point.x), y((float)point.y) {}
	PointF(const Vec2& vec) : x((float)vec.x), y((float)vec.y) {}
	float x;
	float y;
	void moveBy(const Vec2& delta) { x += delta.x; y += delta.y; }
	const Vec2&& movedBy(const Vec2& delta) const { return Vec2(x + delta.x, y + delta.y); }
	const Vec2&& asVec2() { return Vec2(x, y); }
	const Point&& asPoint() { return Point(x, y); }
};


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
	PointF posF;
	Collider collider;
	Collider bottom;
	int ground_y; //�n�ʂ̈ʒu
	bool dead; //���S(�����������͂��Ȃ�)
	int dead_t; //���S��̃^�C�����O
	bool toErase; //��������
	int h_speed, v_speed;
	static const int g;
	int t; //�`��p�ϐ�
	int anim_coef; // �A�j���[�V�������x�̌W��
	bool bound; //ႂ��������Ă��邩�ǂ���
	int bound_t;
	const int bound_limit;
	
	bool isOnGround(); //�n�ʂɑ������Ă��邩
	bool isOnFloor(); //�n��(�K�i�܂�)�ɑ������Ă��邩
	virtual TouchedBlock touchesOn() = 0;
	virtual void goForward() = 0;
	virtual void goBack() = 0;
	virtual void climb() = 0;
	virtual void attack() = 0;
	virtual void wait() = 0;

public:
	Enemy(int hp_, int speed_, const Point& pos_, Wall& wall, Collider& ground, const std::pair<Collider&, Damageable&>& fortress, EnemyManager& manager_);
	virtual ~Enemy() = default;

	virtual void update() = 0;
	virtual void draw() const = 0;
	bool isDead() const { return dead; }
	bool isToErase() { return toErase; }
	bool isAssimilating() const { return state == State::Assimilating; }
	void kill() { dead = true; }
	void bind() { bound = true; bound_t = 0; }
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
	int strength;
	int climb_count;
	Point climbPos; //climb���̕`��ʒu

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
	static const int Size() { return size; }
};



class EnemyManager {
private:
	Wall& wall;
	Collider& ground;
	pair<Collider&, Damageable&> fortress;

	Stopwatch timer;

	list<shared_ptr<Enemy>> enemies;
	Point frontPos; //�O���̈ʒu
	int frontCol; //�O���̗�ԍ�

	const int wait_time;

	void generate();

public:
	EnemyManager(Wall& wall_, Collider& ground_, std::pair<Collider&, Damageable&> fortress_);
	~EnemyManager();
	void update();
	void draw() const;

	auto& getEnemies() { return enemies; }
	Point& getFrontPos() { return frontPos; }
	void setFrontPos(const Point& point) { frontPos.set(point); }
	int getFrontCol() { return frontCol; }
	void setFrontCol(int new_col) { frontCol = new_col; }

	Queue<Thumb*> wait_queue;
};
