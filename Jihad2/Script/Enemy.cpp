#include "Enemy.h"


Enemy::Enemy(int speed_, Wall& wall_)
	:pos(1500, 550),
	collider(pos),
	dead(false),
	invincible(false),
	h_speed(speed_),
	v_speed(0),
	wall(wall_),
	t(0),
	anim_coef(5)
{}



Thumb::Thumb(Wall& wall)
	:Enemy(3, wall),
	touched_wall_pos(0, 0)
{
	const int space = 5;
	collider.add(Rect(pos.movedBy(space*2, space), size - 4*space, size - Chip::size - space));
	collider.add(Rect(pos.movedBy(space, size - Chip::size), size - 2*space, Chip::size - space));
}


void Thumb::move() {
	pos.moveBy(-speed, 0);
	if ((++t / anim_coef) >= 6) t = 0;

	auto& arr = wall.getChips();
	for (int i = 0; i < arr.size(); i++) {
		for (int j = 0; j < arr[i].size(); j++) {
			if (arr[i][j].has_value() && collider.intersects(arr[i][j].value().getCollider())) {
				pos.x = arr[i][j].value().getPos().x + Chip::size; //ï‚ê≥
				if (arr[i-1][j].has_value()) {
					changeState(State::Assimilating);
					invincible = true;
					touched_wall_pos.set(i ,j);
				}
				else changeState(State::Climbing), Println(L"climbed");
			}
		}
	}
}

void Thumb::asslimilate() {
	if ((++t / anim_coef) >= 14) {
		t = 0;
		dead = true;
		wall.extend(touched_wall_pos.x, touched_wall_pos.y+1);
	}
}

void Thumb::climb() {
	if ((++t / anim_coef) >= 10) {
		changeState(State::Normal);
		pos.moveBy(-Chip::size, -Chip::size);
	}
}


void Thumb::update() {

	switch (state)
	{
	case Thumb::State::Normal: 	move(); break;
	case Thumb::State::Assimilating: asslimilate(); break;
	case Thumb::State::Climbing: climb(); break;
	case Thumb::State::Attacking: //çUåÇ break;
	default: break;
	}
	collider.update();
	
}

void Thumb::draw() const {

	for (const auto& elm : collider.get()) elm.draw(Palette::Red);

	int index;
	switch (state)
	{
	case Thumb::State::Normal:
		index = t / anim_coef;
		TextureAsset(L"walk")(index*size, 0, size, size).draw(pos);
		break;
	case Thumb::State::Assimilating:
		index = t / anim_coef;
		TextureAsset(L"change")((index%7)*size, (index/7)*size, size, size).draw(pos);
		break;
	case Thumb::State::Climbing: //ìoí∏ break;
	case Thumb::State::Attacking: //çUåÇ break;
	default: break;
	}

}