#include "Game.h"



Game::Game()
	:ground(Point(0, 0)),
	fortress(wall, ground),
	enemyManager(wall, ground, { fortress.getCollider(), fortress }),
	camera(),
	bg(),
	timer(true),
	timeFont(27, L"Chiller"),
	end(false),
	timeLimit(5),
	pause(false),
	pauseChoice(PauseChoices::Resume),
	pauseFont1(40, L"Haettenschweiler"),
	pauseFont2(30, L"Haettenschweiler"),
	pauseChoiceColor(0,0,0)
{
	fortress.setEneies(enemyManager.getEnemies());
	ground.add(Rect(-100, 700, 5000, 900));

	pauseChoiceStr[0] = L"Resume";
	pauseChoiceStr[1] = L"Back to title";
}


Game::~Game()
{
}


void Game::update()
{
	//ポーズ画面
	if (pause) {
		if (Input::KeyP.clicked) {
			pause = false;
			timer.resume();
		}

		static double t = -PiF / 0;
		if (Input::KeyUp.clicked) {
			pauseChoice = static_cast<PauseChoices>(static_cast<int>(pauseChoice) - 1);
			t = 0;
		}
		else if (Input::KeyDown.clicked) {
			pauseChoice = static_cast<PauseChoices>(static_cast<int>(pauseChoice) + 1);
			t = 0;
		}
		else if (Input::KeyEnter.clicked) {
			switch (pauseChoice)
			{
			case Game::PauseChoices::Resume:
				pause = false;
				timer.resume();
				break;
			case Game::PauseChoices::BackToTile:
				changeScene(SceneName::Title);
				break;
			default:
				break;
			}
		}
		
		//選択中の色
		if ((t += 0.1) >= TwoPi) t = 0;
		pauseChoiceColor.v = 0.5 * sinf(t) + 0.5;


		return;
	}


	if (!end) {

		if (Input::KeyP.clicked) {
			pause = true;
			timer.pause();
			return;
		}

		if (timer.min() >= timeLimit || Input::KeyC.clicked) {
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

		TextureAsset(L"ground").scale(3.0).draw(ground.getOne()->pos.movedBy(-100, 0));
		fortress.draw();
		wall.draw();
		enemyManager.draw();

		ymds::EventManager::get().draw();
	}

	timeFont(timeLimit-1 - timer.min(), L":", Pad(59 - (timer.s() % 60), { 2, L'0' })).drawAt(Window::Center().x, 30, Palette::Black);
	

	//ポーズ画面
	if (pause) {
		static const auto size = 0.6*Window::Size();
		static const auto pos = Window::Center() - size / 2;
		RoundRect(pos.x, pos.y, size.x, size.y, size.x/25).draw(Color(0, 100, 100, 150));

		pauseFont1(L"paused").drawAt(Window::Center().movedBy(0, -170));
		int index = static_cast<int>(pauseChoice);
		for (auto i : step(0, 2)) {
			const auto&& color = i == index ? Color(pauseChoiceColor) : Palette::Black;
			pauseFont2(pauseChoiceStr[i]).drawAt(Window::Center().movedBy(0, i*100), color);
		}
	}
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