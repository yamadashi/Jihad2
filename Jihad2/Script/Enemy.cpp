#include "Enemy.h"


Enemy::Enemy(int speed_, Wall& wall_, Collider& ground_)
	:wall(wall_),
	ground(ground_),
	pos(1500, 550),
	collider(pos),
	bottom(pos),
	ground_y(700),
	dead(false),
	invincible(false),
	h_speed(speed_),
	v_speed(0),
	g(1),
	t(0),
	anim_coef(6)
{}

bool Enemy::onGround()
{
	if (bottom.intersects(ground)) {
		ground_y = ground.get()[0].y;
		return true;
	}
	for (const auto& arr : wall.getChips()) {
		for (const auto& elm : arr) {
			if (elm.has_value() && bottom.intersects(elm.value().getCollider())) {
				ground_y = elm.value().getPos().y;
				return true;
			}
		}
	}
	return false;
}



Thumb::Thumb(Wall& wall, Collider& ground)
	:Enemy(3, wall, ground),
	touched_wall_pos(0, 0)
{
	const int space = 5;
	collider.add(Rect(pos.movedBy(space * 2, space), size - 4 * space, size - Chip::size - space));
	collider.add(Rect(pos.movedBy(space, size - Chip::size), size - 2 * space, Chip::size - space));
	bottom.add(Rect(pos.movedBy(space * 2, size - 1), size - 4 * space, 1));
}


void Thumb::move()
{
	v_speed += g;

	pos.moveBy(-h_speed, v_speed);
	if (onGround()) { v_speed = 0; pos.y = ground_y - Thumb::size; }

	if ((++t / anim_coef) >= 6) t = 0;

	//•Ç‚ÉÚG‚µ‚½‚Æ‚«‚Ì”»’è
	auto& arr = wall.getChips();
	for (int i = 0; i < arr.size(); i++) {
		for (int j = 0; j < arr[i].size(); j++) {
			if (arr[i][j].has_value() && collider.intersects(arr[i][j].value().getCollider())) {
				pos.x = arr[i][j].value().getPos().x + Chip::size; //•â³
				if (arr[i - 1][j].has_value()) {
					changeState(State::Assimilating);
					invincible = true;
					touched_wall_pos.set(i, j);
				}
				else {
					pos.moveBy(-Chip::size, -Chip::size);
					changeState(State::Climbing);
				}
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
	if ((++t / anim_coef) >= 12) {
		changeState(State::Normal);
	}
}


void Thumb::update() {

	switch (state)
	{
	case Thumb::State::Normal: 	move(); break;
	case Thumb::State::Assimilating: asslimilate(); break;
	case Thumb::State::Climbing: climb(); break;
	case Thumb::State::Attacking: //UŒ‚ break;
	default: break;
	}
	collider.update();
	bottom.update();
	
}

void Thumb::draw() const {
	
	int index;
	const int climb_size = 225;

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
	case Thumb::State::Climbing:
		index = t / anim_coef;
		TextureAsset(L"climb")((index % 6)*climb_size, (index / 6)*climb_size, climb_size, climb_size).draw(pos);
		break;
	case Thumb::State::Attacking: //UŒ‚ break;
	default: break;
	}
}


const int Thumb::size = 150;