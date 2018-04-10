# include "Bullet.h"


Explosion::Explosion()
	:Event(),
	t()
{}

void Explosion::update() {}

void Explosion::draw() const {}


Bullet::Bullet(const Point& pos_, float speed, float angle, list<unique_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_)
	:pos(pos_.movedBy(-size/2, -size/2)),
	posF(pos),
	v_x(speed*cosf(angle)),
	v_y(speed*sinf(angle)),
	g(1),
	dead(false),
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

	return false;
}

bool Bullet::isOutOfView() const {
	if (pos.x > Window::Width() || pos.y > Window::Height()) return true;
	return false;
}

void Bullet::onTouch() {
	dead = true;
	//ymds::EventManager::get().registerEvent();
}


vector<Point> Bullet::getTrajectory() const
{
	vector<Point> trajectory;
	PointF posf(pos.movedBy(size/2, size/2));
	float v_y_ = v_y;

	while (v_y_ < 10.0f) {

		v_y_ += g;

		posf.x += v_x;
		posf.y += v_y_;
		
		trajectory.emplace_back((int)posf.x, (int)posf.y);
	}

	return trajectory;
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

	if (isOutOfView()) dead = true;
	if (intersects()) onTouch();
}

void Eye::draw() const
{
	TextureAsset(L"eye").draw(pos);
}

const int Bullet::size = 80;