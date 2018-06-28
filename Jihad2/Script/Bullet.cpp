# include "Bullet.h"


Bullet::Bullet(const Point& pos_, const int size, const Vec2& v0, list<shared_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_)
	:Event(),
	pos(pos_.movedBy(-size / 2, -size / 2)),
	posF(pos),
	v(v0),
	g(1),
	collider(pos),
	enemies(enemies_),
	wall(wall_),
	ground(ground_)
{}

Bullet::Bullet(const Point& pos_, const int size, float speed, float angle, list<shared_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_)
	:Bullet(pos_,size, Vec2(speed*cosf(angle), speed*sinf(angle)),enemies_, wall_, ground_ )
{}


void Bullet::move()
{
	v.y += g;

	posF.moveBy(v);
	pos.set((int)posF.x, (int)posF.y);
}

void Bullet::move(const Vec2& delta)
{
	posF.moveBy(delta);
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
	if (pos.x > ground.getOne()->w || pos.y > ground.getOne()->h) return true;
	return false;
}



Eye::Eye(const Point& pos_, float speed, float angle, list<shared_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_)
	:Bullet(pos_, size, speed, angle, enemies_, wall_, ground_),
	exploding(false)
{
	collider.add(Rect(Pos(), size));
}

vector<Point> Eye::getTrajectory() const
{
	vector<Point> trajectory;
	PointF posf(Pos().movedBy(size/2, size/2));
	Vec2 v = Velocity();

	while (v.y < 10.0f) {

		v.y += g;

		posf.x += v.x;
		posf.y += v.y;
		
		trajectory.emplace_back((int)posf.x, (int)posf.y);
	}

	return trajectory;
}

void Eye::explode() {
	//TinyEye‚Ì¶¬
	const auto&& center = Pos().movedBy(size / 2, size / 2);
	const float rt2 = sqrtf(2);
	for (int i = 0; i < 3; i++) {
		double pw_coef = (i + 1)*0.8;
		EventManager::get().registerEvent(new TinyEye(center, Velocity(), pw_coef*power, i * PiF / 12, enemies, wall, ground));
		EventManager::get().registerEvent(new TinyEye(center.movedBy(0, -rt2 * TinyEye::size), Velocity(), pw_coef*power, -PiF / 2 + i * PiF / 12, enemies, wall, ground));
		EventManager::get().registerEvent(new TinyEye(center.movedBy(0, rt2*TinyEye::size), Velocity(), pw_coef*power, PiF / 2 + i * PiF / 12, enemies, wall, ground));
		EventManager::get().registerEvent(new TinyEye(center.movedBy(-rt2 * TinyEye::size, 0), Velocity(), pw_coef*power, PiF + i * PiF / 12, enemies, wall, ground));
		EventManager::get().registerEvent(new TinyEye(center.movedBy(rt2*TinyEye::size, 0), Velocity(), pw_coef*power, i * PiF / 12, enemies, wall, ground));
		EventManager::get().registerEvent(new TinyEye(center.movedBy(-size / rt2, -size / rt2), Velocity(), pw_coef*power, -3 * PiF / 4 + i * PiF / 12, enemies, wall, ground));
		EventManager::get().registerEvent(new TinyEye(center.movedBy(size / rt2, -size / rt2), Velocity(), pw_coef*power, -PiF / 4 + i * PiF / 12, enemies, wall, ground));
		EventManager::get().registerEvent(new TinyEye(center.movedBy(-size / rt2, size / rt2), Velocity(), pw_coef*power, 3 * PiF / 4 + i * PiF / 12, enemies, wall, ground));
		EventManager::get().registerEvent(new TinyEye(center.movedBy(size / rt2, size / rt2), Velocity(), pw_coef*power, PiF / 4 + i * PiF / 12, enemies, wall, ground));
	}
	kill();
}

void Eye::onTouch() {
	const Vec2&& delta = -Velocity().normalized();
	while (intersects()) {
		move(delta);
		collider.update();
	}
	explosion = std::make_unique<AnimationGIFStrategy>(explosionGIF, Pos(), 1.0, 0.5);
	exploding = true;
}

void Eye::update()
{
	if (!exploding) {
		move();
		collider.update();

		if (isOutOfView()) kill();
		if (intersects()) onTouch();
	}
	else {
		if (explosion->update()) {
			kill();
			explode();
		}
	}
}

void Eye::draw() const
{
	if (!exploding) TextureAsset(L"eye").draw(Pos());
	else explosion->draw();
}


AnimationGIFData Eye::explosionGIF;
const float Eye::power = 7;
const int Eye::size = 100;



TinyEye::TinyEye(const Point& pos_,const Vec2& v0, float speed, float angle, list<shared_ptr<Enemy>>& enemies_, Wall& wall_, Collider& ground_)
	:Bullet(pos_, size, speed, angle, enemies_, wall_, ground_)
{
	collider.add(Rect(Pos(), size));
}

bool TinyEye::intersects() {

	for (const auto& enemy : enemies) {
		if (collider.intersects(enemy->getCollider())) {
			if (!enemy->isDead() && !enemy->isAssimilating()) enemy->kill();
			return true;
		}
	}

	if (collider.intersects(ground)) return true;

	for (const auto& arr : wall.getChips()) {
		for (const auto& chip : arr) {
			if (chip.has_value() && collider.intersects(chip.value().getCollider())) return true;
		}
	}

	return false;
}

void TinyEye::update() {

	move();
	collider.update();

	if (intersects() || isOutOfView()) onTouch();
	
}

void TinyEye::draw() const
{
	TextureAsset(L"tiny_eye").draw(Pos());
}

void TinyEye::onTouch() {
	kill();
}

const int TinyEye::size = 20;//Eye::size / (2*sqrtf(2.0f)+1);