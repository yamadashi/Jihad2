#include "Fortress.h"



Fortress::Fortress(list<unique_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_)
	:Damageable(100),
	pos(50, 450),
	angle(0),
	angle_offset(-5*Pi/16),
	max_angle(0.2), min_angle(-0.5),
	rotation_point(80,195),
	muzzle_offset(242, 470),
	muzzle_point(muzzle_offset),
	rotation_velo(0.025),
	collider(Point(pos)),
	size(250),
	power(5),
	max_power(50),
	clickPos(Mouse::PosF()),
	enemies(enemies_),
	wall(wall_),
	ground(ground_)
{
	collider.add(Rect(pos, 200, size));
}

void Fortress::rotate()
{
	if (Input::MouseL.clicked)
	{
		clickPos.set(Mouse::PosF());
	}
	if (Input::MouseL.pressed)
	{
		Circle(clickPos, 6).draw(Palette::Orange);
		const auto&& diff = Mouse::PosF() - clickPos;
		const double length = diff.length();
		if (length < 20.0) return;
		power = length * 0.25f;
		if (power > max_power) power = max_power;
		angle = -atan2f(diff.x, diff.y) - PiF/4;
		muzzle_point = (muzzle_offset - (pos + rotation_point)).rotated(angle) + (pos + rotation_point);
	
		PutText(angle).from(0, 0);


		/*if (Input::KeyDown.pressed && angle < max_angle) {
			angle += rotation_velo;
			muzzle_point = (muzzle_offset - (pos + rotation_point)).rotated(angle) + (pos + rotation_point);
		}
		if (Input::KeyUp.pressed && angle > min_angle) {
			angle -= rotation_velo;
			muzzle_point = (muzzle_offset - (pos + rotation_point)).rotated(angle) + (pos + rotation_point);
		}*/
	}
}


void Fortress::fire()
{
	if (Input::MouseL.released) {
		bullets.emplace_back(new Eye(muzzle_point.asPoint(), power, angle + angle_offset, enemies, wall, ground));
		power = 5;
		angle = 0;
	}
}

void Fortress::update()
{
	rotate();
	fire();

	for (auto&& bullet : bullets) bullet->update();
	bullets.remove_if([](const unique_ptr<Bullet>& bullet) { return bullet->isDead(); });
}

void Fortress::draw() const {
	
	TextureAsset(L"ashi").draw(pos);
	TextureAsset(L"close").rotateAt(rotation_point, angle).draw(pos);
	PutText(hp).from(pos);
	for (const auto& bullet : bullets) bullet->draw();
	
	if (Input::MouseL.pressed) {
		for (const auto& elm : Eye(muzzle_point.asPoint(), power, angle + angle_offset, enemies, wall, ground).getTrajectory()) {
			Circle(elm.x, elm.y, 3).draw();
		}
	}
}
