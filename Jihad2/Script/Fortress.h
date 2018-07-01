#pragma once
#include "Bullet.h"
#include "Collider.h"
#include "Damageable.h"


using namespace std;

class Fortress : public Damageable
{
private:
	const Point pos;
	double angle; //回転角
	const double angle_offset; //回転角のオフセット
	const double max_angle, min_angle; //回転角の上下限
	const Vec2 rotation_point; //回転の基準点
	const Vec2 muzzle_offset; //マズルの位置計算用のオフセット
	Vec2 muzzle_point; //マズルの位置
	const double rotation_velo; //角速度
	const int size;
	float power; //弾の発射速度
	const float max_power; //弾の発射速度上限
	Vec2 clickPos;
	bool fireReady;
	function<void(Fortress&)> update_func; //update関数の中身
	int hp_bar_show_time;

	Collider collider;
	Optional<list<shared_ptr<Enemy>>&> enemies; //fortressを渡すために初期化タイミングを遅らせています
	Wall& wall;
	Collider& ground;

	void rotate();
	void fire();
	void charge(); //非同期にするかも
	void normalUpdate(); //通常時のupdate関数
	void resetUpdate(); //復帰時のupdate関数
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
