#include "Game.h"



Game::Game()
	:ground(Point(0, 0))
{
	ground.add(Rect(0, 700, 1500, 100));
}


Game::~Game()
{
}


void Game::update()
{
	if (Input::KeySpace.clicked) enemies.emplace_back(new Thumb(wall, ground));

	fortress.update();
	for (auto&& enemy : enemies) enemy->update();

	enemies.remove_if([](const std::unique_ptr<Enemy>& enemy) { return enemy->isDead(); });

}


void Game::draw() const
{
	fortress.draw();
	wall.draw();
	for (const auto& enemy : enemies) enemy->draw();
}