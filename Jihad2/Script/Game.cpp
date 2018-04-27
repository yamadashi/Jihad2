#include "Game.h"



Game::Game()
	:ground(Point(0, 0)),
	fortress(wall, ground),
	enemyManager(wall, ground, { fortress.getCollider(), fortress }),
	camera()
{
	fortress.setEneies(enemyManager.getEnemies());
	ground.add(Rect(-100, 700, 5000, 900));
}


Game::~Game()
{
}


void Game::update()
{
	fortress.update();

	enemyManager.update();
	//ymds::EventManager::get().update();

	camera.update();
}


void Game::draw() const
{
	static const auto& texture = TextureAsset(L"garakuta");
	static const double aspect_ratio = 1 / 0.3281;

	TextureAsset(L"background").draw();
	{
		const auto t = camera.createTransformer();

		TextureAsset(L"garakuta").scale(aspect_ratio).draw(-100, 700 - texture.height*aspect_ratio);
		ground.getOne()->draw();
		fortress.draw();
		wall.draw();
		enemyManager.draw();

		//ymds::EventManager::get().draw();
	}
}