#include "Game.h"



Game::Game()
	:ground(Point(0, 0)),
	fortress(enemies, wall, ground)
{
	ground.add(Rect(0, 700, 1500, 100));
}


Game::~Game()
{
}


void Game::update()
{
	if (Input::KeyEnter.clicked) enemies.emplace_back(new Thumb(wall, ground, { fortress.getCollider(), fortress }));

	fortress.update();
	for (auto&& enemy : enemies) enemy->update();

	enemies.remove_if([](const std::unique_ptr<Enemy>& enemy) { return enemy->isDead(); });

}


void Game::draw() const
{
	for (const auto& elm : ground.get()) elm.draw();
	fortress.draw();
	wall.draw();
	for (const auto& enemy : enemies) enemy->draw();
}