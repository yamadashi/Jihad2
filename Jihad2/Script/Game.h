#pragma once
#include "Scene.h"
# include "Fortress.h"
# include "Wall.h"
# include "Enemy.h"



class Game : public MyScene {
private:
	Fortress fortress;
	Wall wall;
	std::list<std::unique_ptr<Enemy>> enemies;

public:
	Game();
	~Game();

	void update() override;
	void draw() const override;
};

