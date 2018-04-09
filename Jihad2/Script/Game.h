#pragma once
# include "Scene.h"
# include "Fortress.h"
# include "Wall.h"
# include "Enemy.h"


class Game : public MyScene {
private:
	std::list<std::unique_ptr<Enemy>> enemies;
	Collider ground;
	Wall wall;
	Fortress fortress;

public:
	Game();
	~Game();

	void update() override;
	void draw() const override;
};

