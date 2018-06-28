#include "Game.h"



Game::Game()
	:ground(Point(0, 0)),
	fortress(wall, ground),
	enemyManager(wall, ground, { fortress.getCollider(), fortress }),
	camera(),
	bg(),
	timer(true),
	timeFont(25),
	end(false)
{
	fortress.setEneies(enemyManager.getEnemies());
	ground.add(Rect(-100, 700, 5000, 900));
}


Game::~Game()
{
}


void Game::update()
{
	if (!end) {
		if (timer.min() >= 3 || Input::KeyC.clicked) {
			end = true;
			m_data->cleared = true;
		}
		if (fortress.isDead()) {
			end = true;
			m_data->cleared = false;
		}

		fortress.update();

		enemyManager.update();
		ymds::EventManager::get().update();
		bg.update();

		camera.update();
	}
	else {
		changeScene(SceneName::Result);
	}
}


void Game::draw() const
{
	{
		const auto t = camera.createTransformer();

		bg.drawBG();
		bg.drawCloud();
		bg.drawGarakuta();
		bg.drawSmoke();

		ground.getOne()->draw();
		fortress.draw();
		wall.draw();
		enemyManager.draw();

		ymds::EventManager::get().draw();
	}

	timeFont(2 - timer.min(), L":", Pad(59 - (timer.s() % 60), { 2, L'0' })).drawAt(Window::Center().x, 20, Palette::Black);
}



BackGround::BackGround()
	:aspect_ratio(1 / 0.3281),
	cloud_velocity(-1, 0),
	smoke_velocity(2, 0)
{
	for (int i = 0; i < 4; i++) {
		cloud_pos.at(i).set(i * 500 * aspect_ratio, 700 - TextureAsset(L"bg_cloud").height*aspect_ratio);
		smoke_pos.at(i).set(i * 500 * aspect_ratio, 700 - TextureAsset(L"bg_smoke").height*aspect_ratio);
	}
}

void BackGround::update() {

	const auto& cloud = TextureAsset(L"bg_cloud");
	const auto& smoke = TextureAsset(L"bg_smoke");
	for (int i = 0; i < 4; i++) {
		if (cloud_pos.at(i).x + cloud.width*aspect_ratio < -100) {
			cloud_pos.at(i).set(cloud_pos.at((i+3)%4).movedBy(cloud.width*aspect_ratio, 0));
		}
		if (smoke_pos.at(i).x > Window::Width()*aspect_ratio) {
			smoke_pos.at(i).set(smoke_pos.at((i+1)%4).movedBy(-smoke.width*aspect_ratio, 0));
		}
		cloud_pos.at(i).moveBy(cloud_velocity);
		smoke_pos.at(i).moveBy(smoke_velocity);
	}
}

void BackGround::drawBG() const {

	const auto& bg = TextureAsset(L"background");
	bg.scale(aspect_ratio).draw(-100, 700 - bg.height*aspect_ratio);
}

void BackGround::drawGarakuta() const {
	
	const auto& garakuta = TextureAsset(L"garakuta");
	garakuta.scale(aspect_ratio).draw(-100, 700 - garakuta.height*aspect_ratio);
}

void BackGround::drawCloud() const {

	const auto& cloud = TextureAsset(L"bg_cloud");
	for (int i = 0; i < cloud_pos.size(); i++) {
		cloud.scale(aspect_ratio).draw(cloud_pos.at(i));
	}
}

void BackGround::drawSmoke() const {
	
	const auto& smoke = TextureAsset(L"bg_smoke");
	for (int i = 0; i < smoke_pos.size(); i++) {
		smoke.scale(aspect_ratio).draw(smoke_pos.at(i));
	}
}