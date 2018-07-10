#pragma once
#include "Bullet.h"
#include "Collider.h"
#include "Damageable.h"


using namespace std;

class Fortress : public Damageable
{
private:
	Collider collider;
	Optional<list<shared_ptr<Enemy>>&> enemies; //fortress��n�����߂ɏ������^�C�~���O��x�点�Ă��܂�
	Wall& wall;
	Collider& ground;

	const Point pos;
	const int size;


	double angle; //��]�p
	const double angle_offset; //��]�p�̃I�t�Z�b�g
	const double max_angle, min_angle; //��]�p�̏㉺��
	const Vec2 rotation_point; //��]�̊�_
	const Vec2 muzzle_offset; //�}�Y���̈ʒu�v�Z�p�̃I�t�Z�b�g
	Vec2 muzzle_point; //�}�Y���̈ʒu
	const double rotation_velo; //�p���x
	
	float power; //�e�̔��ˑ��x
	const float max_power; //�e�̔��ˑ��x���

	Vec2 clickPos;
	const Key* pressedKey;

	bool fireReady;
	bool charged;
	int charge_t;
	const int charge_wait_time;
	bool mouth_open;
	int wink_t;
	
	function<void(Fortress&)> update_func; //update�֐��̒��g
	
	int hp_bar_show_time;


	void rotate();
	void fire();
	void charge();
	void normalUpdate(); //�ʏ펞��update�֐�
	void resetUpdate(); //���A����update�֐�
	void onDamaged();

public:
	Fortress(Wall&, Collider& ground_);
	~Fortress() = default;

	void update();
	void draw() const;
	Collider& getCollider() { return collider; }
	void damage(int val) override { hp -= val; onDamaged(); }
	void setEneies(list<shared_ptr<Enemy>>& enemies_) { enemies.emplace(enemies_); }
	bool isDead() const { return hp <= 0; }
};
