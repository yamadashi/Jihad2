#include "Fortress.h"



Fortress::Fortress(Wall& wall_, Collider& ground_)
	:Damageable(100),
	pos(50, 462),
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
	pressedKey(nullptr),
	fireReady(false),
	charged(true),
	charge_t(0),
	charge_wait_time(480),
	mouth_open(false),
	wink_t(0),
	update_func(&Fortress::normalUpdate),
	wall(wall_),
	ground(ground_)
{
	collider.add(Rect(pos, 200, size));
}

void Fortress::rotate()
{
	static bool clicked = false;

	
	if (Input::MouseL.clicked)
	{
		clickPos.set(Mouse::PosF());
		clicked = true;
		pressedKey = &Input::MouseL;
	}
	else if (Input::MouseR.clicked)
	{
		clickPos.set(Mouse::PosF());
		clicked = true;
		pressedKey = &Input::MouseR;
	}


	if (pressedKey != nullptr && pressedKey->released) {
		clicked = false;
		return;
	}

	if (clicked && pressedKey->pressed)
	{
		Circle(clickPos, 6).draw(Palette::Orange);
		const auto&& diff = Mouse::PosF() - clickPos;
		const double length = diff.length();
		if (length < 20.0) {
			fireReady = false;
			return;
		}
		fireReady = true;
		power = length * 0.25f;
		if (power > max_power) power = max_power;
		angle = -atan2f(diff.x, diff.y) - PiF/4;
		muzzle_point = (muzzle_offset - (pos + rotation_point)).rotated(angle) + (pos + rotation_point);
	}
}


void Fortress::fire()
{
	if (pressedKey != nullptr && pressedKey->released) {

		//–Ú
		if (*pressedKey == Input::MouseL) {
			if (!charged || !fireReady) return;

			EventManager::get().registerEvent(new Eye(muzzle_point.asPoint(), power, angle + angle_offset, *enemies, wall, ground));
			power = 5;
			mouth_open = true;
			update_func = &Fortress::resetUpdate;
			fireReady = false;
			charged = false;
			charge_t = 0;
		}
		//á‚
		else if (*pressedKey == Input::MouseR) {
			if (!fireReady) return;

			EventManager::get().registerEvent(new Sputum(muzzle_point.asPoint(), power, angle + angle_offset, *enemies, wall, ground));
			power = 5;
			mouth_open = true;
			update_func = &Fortress::resetUpdate;
			fireReady = false;
			charge_t = 0;
		}

		pressedKey = nullptr;
	}
}

void Fortress::charge()
{
	if (charged) return;

	if (++charge_t >= charge_wait_time) {
		charged = true;
	}
}

void Fortress::normalUpdate()
{
	rotate();
	charge();
	fire();
}

void Fortress::resetUpdate()
{
	static int t = 0;
	const float delta = -0.01*(angle/fabsf(angle));
	angle += delta;
	if (fabsf(angle) < delta) {
		angle = 0;
		update_func = &Fortress::normalUpdate;
		t = 0;
		mouth_open = false;
	}
}

void Fortress::onDamaged() {
	hp_bar_show_time = 150;
}


void Fortress::update()
{
	update_func(*this);
	if (hp_bar_show_time > 0) hp_bar_show_time--;
	
	if (!charged) {
		if ((wink_t / 7) < 3) wink_t++;
		Println(wink_t/7);
	}
	else {
		if ((wink_t / 7) > 0) wink_t--;
	}
}

void Fortress::draw() const {
	
	TextureAsset(L"ashi").draw(pos);
	TextureAsset(charged ? L"eye_filled" : L"eye_empty").rotateAt(rotation_point, angle).draw(pos);
	if (mouth_open) TextureAsset(L"open"+ToString(wink_t/7)).rotateAt(rotation_point, angle).draw(pos);
	else TextureAsset(L"close"+ToString(wink_t/7)).rotateAt(rotation_point, angle).draw(pos);

	//‹O“¹‚ð•\Ž¦
	if (fireReady && pressedKey != nullptr && pressedKey->pressed) {
		for (const auto& elm : Eye(muzzle_point.asPoint(), power, angle + angle_offset, *enemies, wall, ground).getTrajectory()) {
			Circle(elm.x, elm.y, 3).draw(Palette::Red);
		}
	}

	//HPƒo[
	static const int hp_bar_width = 20, hp_bar_length = 100;
	static const Point hp_bar_pos(pos.movedBy(60, -55));
	
	if (hp_bar_show_time > 0) {
		Rect(hp_bar_pos.movedBy(-2, -2), hp_bar_length + 4, hp_bar_width + 4).draw(Palette::Gray);
		Rect(hp_bar_pos, hp_bar_length*((float)hp / max_hp), hp_bar_width).draw(Palette::Red);
	}

}
