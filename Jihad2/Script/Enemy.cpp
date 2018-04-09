#include "Enemy.h"


Enemy::Enemy(int speed_, Wall& wall_, Collider& ground_, const std::pair<Collider&, Damageable&>& fortress_)
	:Damageable(5),
	wall(wall_),
	ground(ground_),
	fortress(fortress_),
	pos(1500, 25),
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

bool Enemy::isOnGround()
{
	if (bottom.intersects(ground)) {
		ground_y = ground.getOne()->y;
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



Thumb::Thumb(Wall& wall, Collider& ground, const std::pair<Collider&, Damageable&>& fortress)
	:Enemy(2, wall, ground, fortress),
	touched_wall_pos(0, 0),
	state(State::Moving)
{
	int space = collider_space_diff;
	collider.add(Rect(pos.movedBy(space * 2, space), size - 4 * space, size - Chip::size - space));
	collider.add(Rect(pos.movedBy(space, size - Chip::size), size - 2 * space, Chip::size - space));
	bottom.add(Rect(pos.movedBy(space * 2, size - 1), size - 4 * space, 1));
}


void Thumb::onTouch() {

	//çUåÇîªíË
	if (collider.intersects(fortress.first)) {
		changeState(State::Waiting);
		pos.x = fortress.first.getOne()->x + 200; //ï‚ê≥
		collider.update();
		bottom.update();
		return;
	}
	//ï«Ç∆ÇÃê⁄êGîªíË
	auto& arr = wall.getChips();
	for (int i = 0; i < arr.size(); i++) {
		for (int j = 0; j < arr[i].size(); j++) {
			if (arr[i][j].has_value() && collider.intersects(arr[i][j].value().getCollider())) {
				pos.x = arr[i][j].value().getPos().x + Chip::size; //ï‚ê≥
				if (arr[i - 1][j].has_value()) {
					changeState(State::Assimilating);
					invincible = true;
					touched_wall_pos.set(i, j);
					return;
				}
				else {
					pos.moveBy(-Chip::size, -Chip::size);
					changeState(State::Climbing);
					return;
				}
			}
		}
	}
}


void Thumb::move()
{
	v_speed += g;

	pos.moveBy(-h_speed, v_speed);
	if (isOnGround()) { v_speed = 0; pos.y = ground_y - Thumb::size; } //ï‚ê≥

	if ((++t / anim_coef) >= 6) t = 0;

	onTouch();
	
}

void Thumb::asslimilate()
{
	if ((++t / anim_coef) >= 14) {
		t = 0;
		dead = true;
		wall.extend(touched_wall_pos.x, touched_wall_pos.y+1);
	}
}

void Thumb::climb()
{
	if ((++t / anim_coef) >= 12) {
		changeState(State::Moving);
		//ï«Ç∆ê⁄êGÇ∑ÇÈÇÊÇ§Ç…ï‚ê≥
		pos.moveBy(-collider_space_diff-1, 0); //ç™ãíÇÃîñÇ¢êîíl
		collider.update();
		bottom.update();
		onTouch();
	}
}

void Thumb::attack()
{
	if ((++t / anim_coef) * 1.5 >= 5) {
		changeState(State::Moving);
		fortress.second.damage(5);
		pos.moveBy(-collider_space_diff - 1, 0); //ç™ãíÇÃîñÇ¢êîíl
		collider.update();
		bottom.update();
		onTouch();
	}
}

void Thumb::wait()
{
	static const int wait_time = 5;
	if ((++t / anim_coef) >= wait_time) {
		changeState(State::Attacking);
	}
}

void Thumb::update() {

	switch (state)
	{
	case Thumb::State::Moving: move(); break;
	case Thumb::State::Assimilating: asslimilate(); break;
	case Thumb::State::Climbing: climb(); break;
	case Thumb::State::Attacking: attack(); break;
	case Thumb::State::Waiting: wait(); break;
	default: break;
	}
	collider.update();
	bottom.update();
	
}

void Thumb::draw() const {

	int index;

	switch (state)
	{
	case Thumb::State::Moving:
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
	case Thumb::State::Attacking:
		index = (t / anim_coef) * 1.5;
		TextureAsset(L"attack")((index % 5)*size, (index / 5)*size, size, size).draw(pos);
		break;
	case Thumb::State::Waiting:
		TextureAsset(L"wait").draw(pos);
		break;
	default: break;
	}

}


const int Thumb::size = 150;
const int Thumb::climb_size = 225;
const int Thumb::collider_space_diff = 5;