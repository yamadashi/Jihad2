#include "Enemy.h"


Enemy::Enemy(int speed_, Wall& wall_, Collider& ground_, const std::pair<Collider&, Damageable&>& fortress_, EnemyManager& manager_)
	:Damageable(5),
	state(State::Forward),
	wall(wall_),
	ground(ground_),
	fortress(fortress_),
	manager(manager_),
	pos(1500, 25),
	collider(pos),
	bottom(pos),
	ground_y(700),
	dead(false),
	h_speed(speed_),
	v_speed(0),
	t(0),
	anim_coef(6),
	hasClimbed(false)
{}

bool Enemy::isOnGround()
{
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
bool Enemy::anyoneAssimilating = false;





EnemyManager::EnemyManager(Wall & wall_, Collider & ground_, pair<Collider&, Damageable&> fortress_)
	:wall(wall_),
	ground(ground_),
	fortress(fortress_),
	frontPos(none),
	climb_wait_time(210)
{}

void EnemyManager::generate()
{
	if (Input::KeyEnter.clicked) enemies.emplace_back(new Thumb(wall, ground, fortress, *this));
}

void EnemyManager::update()
{
	generate();
	
	static int wait_t = climb_wait_time;
	if (!climb_queue.empty()) {
		if (wait_t-- <= 0) {
			climb_queue.dequeue()->changeState(Enemy::State::Forward);
			wait_t = climb_wait_time;
		}
	}
	else {
		wait_t = 5; //anyoneisClimbingÇÃÇ∆Ç´à»äOÇÃÇ›Ç…ÇµÇΩÇŸÇ§Ç™ó«Ç¢ÅH
	}

	for (const auto& enemy : enemies) enemy->update();


	//Ç±ÇÃèáî‘Ç∂Ç·Ç»Ç¢Ç∆É_ÉÅ

	climb_queue.remove_if([](const Enemy* elm) { return elm->isDead(); });
	back_queue.remove_if([](const Enemy* elm) { return elm->isDead(); });
	enemies.remove_if([](const shared_ptr<Enemy>& enemy) { return enemy->isDead(); });

}

void EnemyManager::draw() const
{
	for (const auto& enemy : enemies) enemy->draw();
	Circle(frontPos.value_or(Point(0, 0)), 5).draw(Palette::Red);
}






Thumb::Thumb(Wall& wall, Collider& ground, const std::pair<Collider&, Damageable&>& fortress, EnemyManager& manager_)
	:Enemy(2, wall, ground, fortress, manager_),
	touched_wall_pos(0, 0)
{
	int space = collider_space_diff;
	collider.add(Rect(pos.movedBy(space * 2, space), size - 4 * space, size - Chip::size - space));
	collider.add(Rect(pos.movedBy(space, size - Chip::size), size - 2 * space, Chip::size - space));
	bottom.add(Rect(pos.movedBy(space * 2, size - 1), size - 4 * space, 1));
}


void Thumb::onTouch() {

	//ëOê¸Çí¥Ç¶Ç»Ç¢
	if (anyoneAssimilating) {
		if (pos.x < manager.getFrontPos().value_or(Point(0, 0)).x) {
			changeState(State::Back);
			manager.getBackQueue().enqueue(this);
		}
		else {
			state = State::Forward;
		}
	}
	else {
		state = State::Forward;
	}

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
				pos.x = arr[i][j].value().getPos().x + Chip::size; //ï«Ç…ìñÇΩÇ¡ÇΩÇ∆Ç´ÇÃï‚ê≥
				//ìØâª
				if (arr[i - 1][j].has_value()) {
					if (!anyoneAssimilating) { //Ç±ÇÃèåèÇæÇ∆ìØéûÇ…ìØâªÇ≈Ç´Ç»Ç¢
						changeState(State::Assimilating);
						touched_wall_pos.set(i, j);
						anyoneAssimilating = true;
						if (pos.movedBy(Thumb::size, 0).x > manager.getFrontPos().value_or(Point(0, 0)).x) manager.setFrontPos(pos.movedBy(Thumb::size, 0));

						size_t queue_size = manager.getClimbQueue().size();
						for (size_t k = 0; k < queue_size; k++) {
							Enemy* tmp = manager.getClimbQueue().dequeue();
							tmp->changeState(Enemy::State::Back);
							manager.getBackQueue().enqueue(tmp);
						}
						return;
					}
					else {
						manager.getBackQueue().enqueue(this);
						return;
					}
				}
				//ìoÇÈ
				else {
					if (hasClimbed) {
						changeState(State::Climbing);
						climb();
						return;
					}
					else {
						changeState(State::None);
						manager.getClimbQueue().enqueue(this);
						hasClimbed = true;
						return;
					}
				}
			}
		}
	}

}


void Thumb::goForward()
{
	v_speed += g;

	pos.moveBy(-h_speed, v_speed);
	if (isOnFloor()) { v_speed = 0; pos.y = ground_y - Thumb::size; } //ï‚ê≥

	if ((++t / anim_coef) >= 6) t = 0;

	//if (isOnGround()) hasClimbed = false;

	onTouch();
	
}

void Thumb::goBack()
{
	v_speed += g;

	pos.moveBy(h_speed, v_speed);
	if (isOnFloor()) { v_speed = 0; pos.y = ground_y - Thumb::size; } //ï‚ê≥

	if ((++t / anim_coef) >= 6) t = 0;

	if (isOnFloor()) hasClimbed = false;

	if (pos.x > manager.getFrontPos().value_or(Point(0, 0)).x) changeState(State::None);
	//BackÇµÇ´ÇÈÇ‹Ç¶Ç…changeStatee(State::Forward)ÇµÇƒÇ¢ÇÈÇ©ÇÁî˜ñ≠Ç»Ç©ÇÒÇ∂Ç…Ç»ÇÈÅ®isOnFloor()ä÷êîÇ≈Ç»ÇÒÇ∆Ç©Ç»Ç¡ÇΩÅH
}

void Thumb::asslimilate()
{
	if ((++t / anim_coef) >= 14) {
		t = 0;
		dead = true;
		wall.extend(touched_wall_pos.x, touched_wall_pos.y+1);
		anyoneAssimilating = false;

		size_t queue_size = manager.getBackQueue().size();
		for (size_t i = 0; i < queue_size; i++) {
			manager.getBackQueue().dequeue()->changeState(State::Forward);
		}
	}
}

void Thumb::climb()
{
	if (t == 0)
		pos.moveBy(-Chip::size, -Chip::size);

	if ((++t / anim_coef) >= 12) {
		changeState(State::Forward);
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
		changeState(State::Forward);
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
	case Thumb::State::Forward: goForward(); break;
	case Thumb::State::Back: goBack(); break;
	case Thumb::State::Assimilating: asslimilate(); break;
	case Thumb::State::Climbing: climb(); break;
	case Thumb::State::Attacking: attack(); break;
	case Thumb::State::Waiting: wait(); break;
	case Thumb::State::None: break;
	default: break;
	}
	collider.update();
	bottom.update();
	
}

void Thumb::draw() const {

	int index;
	Circle(pos, 5).draw();
	
	//if (onGround) for (const auto& elm : collider.get()) elm.draw(Palette::Red);

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
		TextureAsset(L"climb")((index % 6)*climb_size, (index / 6)*climb_size, climb_size, climb_size).draw(pos);
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

