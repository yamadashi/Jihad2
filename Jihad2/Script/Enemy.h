#pragma once
#include "Collider.h"
#include "Wall.h"
#include "Damageable.h"
# include "ymdsUtility.h"

using namespace ymds;


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
	int ground_y; //�n�ʂ̈ʒu
	bool dead;
	int h_speed, v_speed;
	static const int g;
	int t; //�`��p�ϐ�
	int anim_coef; // �A�j���[�V�������x�̌W��
	bool hasClimbed; //�K�i�����ɓo�������ǂ���
	static bool anyoneAssimilating;
	//static bool anyoneClimbing;
	
	bool isOnGround(); //�n��(�K�i�łȂ�)�ɑ������Ă��邩
	bool isOnFloor(); //�n��(�K�i�܂�)�ɑ������Ă��邩
	virtual void onTouch() = 0;
	virtual void goForward() = 0;
	virtual void goBack() = 0;
	virtual void climb() = 0;
	virtual void attack() = 0;
	virtual void wait() = 0;

public:
	Enemy(int speed_, Wall& wall, Collider& ground, const std::pair<Collider&, Damageable&>& fortress, EnemyManager& manager_);
	virtual ~Enemy() = default;

	virtual void update() = 0;
	virtual void draw() const = 0;
	bool isDead() const { return dead; }
	void damage(int val) override { hp -= val; }
	const Collider& getCollider() const { return collider; }
	const State& getState() const { return state; }
	const Point& getPos() const { return pos; }
	void changeState(const State& new_state) { state = new_state; t = 0; }
};


class EnemyManager {
private:
	Wall& wall;
	Collider& ground;
	pair<Collider&, Damageable&> fortress;

	list<shared_ptr<Enemy>> enemies;
	Queue<Enemy*> climb_queue; //�K�i�̑҂��s��
	Queue<Enemy*> back_queue; //��ނ���Enemy�̃��X�g
	Optional<Point> frontPos; //�O���̈ʒu
	const int climb_wait_time;

	void generate();

public:
	EnemyManager(Wall& wall_, Collider& ground_, std::pair<Collider&, Damageable&> fortress_);
	~EnemyManager() = default;
	void update();
	void draw() const;

	auto& getEnemies() { return enemies; }
	Optional<Point>& getFrontPos() { return frontPos; }
	void setFrontPos(const Point& point) { frontPos.emplace(point); }
	Queue<Enemy*>& getClimbQueue() { return climb_queue; }
	Queue<Enemy*>& getBackQueue() { return back_queue; }
};




class Thumb final : public Enemy {
private:
	static const int size;
	static const int climb_size;
	static const int collider_space_diff;
	Point touched_wall_pos;

protected:
	void onTouch() override;
	void goForward() override;
	void goBack() override;
	void asslimilate();
	void climb() override;
	void attack() override;
	void wait() override;

public:
	Thumb(Wall& wall, Collider& ground, const std::pair<Collider&, Damageable&>& fortress, EnemyManager& manager_);
	~Thumb() = default;

	void update() override;
	void draw() const override;
};

