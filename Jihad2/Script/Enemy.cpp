#include "Enemy.h"


Enemy::Enemy(int speed_, const Point& pos_, Wall& wall_, Collider& ground_, const std::pair<Collider&, Damageable&>& fortress_, EnemyManager& manager_)
	:Damageable(5),
	state(State::Forward),
	wall(wall_),
	ground(ground_),
	fortress(fortress_),
	manager(manager_),
	pos(pos_),
	collider(pos),
	bottom(pos),
	ground_y(700),
	dead(false),
	h_speed(speed_),
	v_speed(0),
	t(0),
	anim_coef(6)
{}

bool Enemy::isOnGround() {
	if (bottom.intersects(ground)) {
		ground_y = ground.getOne()->y;
		return true;
	}
	return false;
}

bool Enemy::isOnFloor() {

	if (isOnGround()) return true;

	for (const auto& arr : wall.getChips()) {
		for (const auto& elm : arr) {
			if (elm.has_value() && bottom.intersects(elm->getCollider())) {
				ground_y = elm->getPos().y;
				return true;
			}
		}
	}
	return false;
}

const int Enemy::g = 1;





EnemyManager::EnemyManager(Wall & wall_, Collider & ground_, pair<Collider&, Damageable&> fortress_)
	:wall(wall_),
	ground(ground_),
	fortress(fortress_),
	timer(true),
	wait_time(180),
	frontPos(fortress.first.getOne().value().pos),
	frontCol(0)
{
	timer.restart();
}

void EnemyManager::generate()
{
	static int pre_s;
	if (timer.s() - pre_s > 2) {
		pre_s = timer.s();
		for (int i = 0; i < 4*(1+sinf(timer.ms()*PiF/30)); i++)
			enemies.emplace_back(new Thumb({ 1500+i*Random<int>(1,10)*15, 100 }, wall, ground, fortress, *this));
	}

	//if (Input::KeyEnter.clicked) enemies.emplace_back(new Thumb({ 1500, 100 }, wall, ground, fortress, *this));
}

void EnemyManager::update()
{
	generate();
	
	static int wait_t = wait_time;
	if (!wait_queue.empty()) {
		if (wait_t-- <= 0) {
			wait_queue.dequeue()->transit();
			wait_t = wait_time;
		}
	}

	for (const auto& enemy : enemies) enemy->update();

	wait_queue.remove_if([](const Thumb* enemy) { return enemy->isDead(); });
	enemies.remove_if([](const shared_ptr<Enemy>& enemy) { return enemy->isDead(); });
}

void EnemyManager::draw() const
{
	for (const auto& enemy : enemies) enemy->draw();
}






Thumb::Thumb(const Point& pos_, Wall& wall, Collider& ground, const std::pair<Collider&, Damageable&>& fortress, EnemyManager& manager_)
	:Enemy(2, pos_, wall, ground, fortress, manager_),
	touchPos(none),
	hasClimbed(false),
	climb_count(0),
	climbPos(pos)
{
	int space = collider_space_diff;
	collider.add(Rect(pos.movedBy(space * 2, space), size - 4 * space, size - Chip::size - space));
	collider.add(Rect(pos.movedBy(space, size - Chip::size), size - 2 * space, Chip::size - space));
	bottom.add(Rect(pos.movedBy(space * 2, size - 1), size - 4 * space, 1));
}


TouchedBlock Thumb::touchesOn() {
	
	//攻撃判定
	if (collider.intersects(fortress.first)) {
		changeState(State::Attacking);
		pos.x = fortress.first.getOne()->x + 200; //補正
		collider.update();
		bottom.update();
		return none;
	}

	if (climb_count >= 8) return none;
	//壁との接触判定
	auto& arr = wall.getChips();
	for (int i = 0; i < arr.size(); i++) {
		for (int j = 0; j < arr[i].size(); j++) {
			//壁に当たった時
			if (arr[i][j].has_value() && collider.intersects(arr[i][j]->getCollider())) {
				pos.x = arr[i][j].value().getPos().x + Chip::size; //壁に当たったときの補正
				return TouchedBlock(in_place, i, j);
			}
		}
	}
	return none;
}


void Thumb::transit() {

	if (touchPos.has_value()) {
		const auto& arr = wall.getChips();
		int row = touchPos->x, col = touchPos->y;
		//同化
		//接触ブロックの上にブロックがあった時
		if (arr[row - 1][col].has_value()) {

			changeState(Enemy::State::Assimilating);
			//前線を形成
			auto new_front = arr[row][col].value().getPos().movedBy(2 * Chip::size - 10, 0);
			if (new_front.x > manager.getFrontPos().x) {
				manager.setFrontPos(new_front);
				manager.setFrontCol(touchPos->y);
			}
		}
		//登る
		else {
			changeState(Enemy::State::Climbing);
			climb_count++;
			setClimbed();
			climb();
		}
	}
}


void Thumb::goForward()
{
	v_speed += g;

	pos.moveBy(-h_speed, v_speed);
	if (isOnFloor()) { v_speed = 0; pos.y = ground_y - Thumb::size; } //補正
	
	//アニメーション
	if ((++t / anim_coef) >= 6) t = 0;

	touchPos = touchesOn();
	if (touchPos.has_value()) {
		if (!hasClimbed) {
			manager.wait_queue.enqueue(this);
			changeState(State::Waiting);
		}
		else {
			transit();
		}
	}
	else if (pos.x < manager.getFrontPos().x && !hasClimbed) {
		changeState(State::Back);
		touchPos.emplace(8, manager.getFrontCol()+1);
		manager.wait_queue.remove_if([this](const Thumb* en) { return en == this; });
	}

}

void Thumb::goBack()
{
	v_speed += g;

	pos.moveBy(h_speed, v_speed);
	if (isOnFloor()) { v_speed = 0; pos.y = ground_y - Thumb::size; } //補正

	if ((++t / anim_coef) >= 6) t = 0;


	int margin = 10; //画像の位置のずれを補正（しょうがないんです）

	if (pos.x > manager.getFrontPos().x+margin) {
		pos.x = manager.getFrontPos().x+margin;
		changeState(State::Waiting);
		manager.wait_queue.enqueue(this);
	}
}

void Thumb::asslimilate()
{
	if ((++t / anim_coef) >= 14) {
		t = 0;
		dead = true;
		wall.extend(touchPos->x, touchPos->y+1);
	}
}

void Thumb::climb()
{
	//描画位置climb
	static Vec2 tmpPos; //posのVec2表現
	static const Vec2 delta = Vec2(-Chip::size, -Chip::size) / double(12*anim_coef);

	if (t == 0) {
		tmpPos.set(pos);
		climbPos = pos.movedBy(-Chip::size, -Chip::size);
	}
	else {
		tmpPos += delta;
		pos = tmpPos.asPoint();
	}

	if ((++t / anim_coef) >= 12) {
		changeState(State::Forward);
		pos.set(climbPos);
		collider.update();
		bottom.update();
	}
}

void Thumb::attack()
{
	if ((++t / anim_coef) * 1.5 >= 5) {
		changeState(State::Forward);
		fortress.second.damage(5);
		pos.moveBy(-collider_space_diff - 1, 0); //根拠の薄い数値
		collider.update();
		bottom.update();
	}
}

void Thumb::wait()
{
	if (pos.x < manager.getFrontPos().x && !hasClimbed) {
		changeState(State::Back);
		manager.wait_queue.remove_if([this](const Thumb* en) { return en == this; });
		touchPos->y++;
	}
}

void Thumb::update() {

	switch (state)
	{
	case Thumb::State::Forward: goForward(); break;
	case Thumb::State::Back: goBack(); break;
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
	case Thumb::State::Forward:
		index = t / anim_coef;
		TextureAsset(L"walk")(index*size, 0, size, size).draw(pos);
		break;
	case Thumb::State::Back:
		index = t / anim_coef;
		TextureAsset(L"walk")(index*size, 0, size, size).mirror().draw(pos);
		break;
	case Thumb::State::Assimilating:
		index = t / anim_coef;
		TextureAsset(L"change")((index%7)*size, (index/7)*size, size, size).draw(pos);
		break;
	case Thumb::State::Climbing:
		index = t / anim_coef;
		TextureAsset(L"climb")((index % 6)*climb_size, (index / 6)*climb_size, climb_size, climb_size).draw(climbPos);
		break;
	case Thumb::State::Attacking:
		index = (int)((t / anim_coef) * 1.5);
		TextureAsset(L"attack")((index % 5)*size, (index / 5)*size, size, size).draw(pos);
		break;
	case Thumb::State::Waiting:
		TextureAsset(L"wait").draw(pos);
		break;
	case Thumb::State::None:
		TextureAsset(L"wait").draw(pos);
		break;
	default: break;
	}

}

const int Thumb::size = 150;
const int Thumb::climb_size = 225;
const int Thumb::collider_space_diff = 5;
//bool Thumb::anyoneAssimilating = false;
