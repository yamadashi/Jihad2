#include "Fortress.h"



Fortress::Fortress(list<unique_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_)
	:Damageable(100),
	pos(50, 450),
	angle(0),
	angle_offset(-5*Pi/16),
	max_angle(0.2), min_angle(-0.5),
	rotation_point(80,195),
	muzzle_offset(242, 470),
	rotation_velo(0.025),
	collider(Point(pos)),
	size(250),
	max_power(50),
	enemies(enemies_),
	wall(wall_),
	ground(ground_)
{
	collider.add(Rect(pos, 200, size));
}

void Fortress::rotate() {

	if (Input::KeyDown.pressed && angle < max_angle) {
		angle += rotation_velo;
	}
	if (Input::KeyUp.pressed && angle > min_angle) {
		angle -= rotation_velo;
	}
}


void Fortress::fire() {
	static float power = 0;
	if (Input::KeySpace.pressed) {
		if ((power += 1) > max_power) power = max_power;
	}
	else if (Input::KeySpace.released) {
		const auto&& muzzle_point = (muzzle_offset - (pos + rotation_point)).rotated(angle) + (pos + rotation_point);
		bullets.emplace_back(new Eye(muzzle_point.asPoint(), power, angle + angle_offset, enemies, wall, ground));
		power = 0;
	}
}

void Fortress::update() {

	rotate();
	fire();

	for (auto&& bullet : bullets) bullet->update();
	bullets.remove_if([](const unique_ptr<Bullet>& bullet) { return bullet->hasTouched(); });
}


void Fortress::draw() const {
	
	TextureAsset(L"ashi").draw(pos);
	TextureAsset(L"close").rotateAt(rotation_point, angle).draw(pos);
	PutText(hp).from(pos);
	for (const auto& bullet : bullets) bullet->draw();
	PutText(L"bullets:", bullets.size()).from(0, 0);
}
