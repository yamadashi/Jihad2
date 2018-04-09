# include "Bullet.h"


Bullet::Bullet(const Point& pos_, float speed, float angle, list<unique_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_)
	:pos(pos_.movedBy(-size/2, -size/2)),
	posF(pos),
	v_x(speed*cosf(angle)),
	v_y(speed*sinf(angle)),
	g(1),
	touched(false),
	collider(pos),
	enemies(enemies_),
	wall(wall_),
	ground(ground_)
{}

void Bullet::move()
{
	v_y += g;

	posF.x += v_x;
	posF.y += v_y;
	pos.set((int)posF.x, (int)posF.y);
}

bool Bullet::intersects() const
{
	if (collider.intersects(ground)) return true;

	for (const auto& arr : wall.getChips()) {
		for (const auto& chip : arr) {
			if (chip.has_value() && collider.intersects(chip.value().getCollider())) return true;
		}
	}

	for (const auto& enemy : enemies) {
		if (collider.intersects(enemy->getCollider())) return true;
	}
}

void Bullet::onTouch() {
	touched = true;
	//generate effect as Event instance
}


Eye::Eye(const Point& pos_, float speed, float angle, list<unique_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_)
	:Bullet(pos_, speed, angle, enemies_, wall_, ground_)
{
	collider.add(Rect(pos, size));
}

void Eye::update()
{
	move();
	collider.update();
	onTouch();
}

void Eye::draw() const {
	TextureAsset(L"eye").draw(pos);
}

const int Bullet::size = 80;