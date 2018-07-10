# include "Tutorial.h"

Tutorial::Tutorial()
	:ground(Point(0, 0)),
	fortress(wall, ground),
	enemyManager(wall, ground, { fortress.getCollider(), fortress }),
	camera(),
	bg(),
	timeFont(27, L"Chiller")
{
	fortress.setEneies(enemyManager.getEnemies());
	ground.add(Rect(-100, 700, 5000, 900));
}


void Tutorial::update() {

	fortress.update();

	enemyManager.update();
	//ymds::EventManager::get().update();
	bg.update();

	camera.update();
}

void Tutorial::draw() const {
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

	timeFont(L"3:00").drawAt(Window::Center().x, 30, Palette::Black);
}
