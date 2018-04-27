#pragma once
# include "Scene.h"
# include "Fortress.h"
# include "Wall.h"
# include "Enemy.h"
# include "MyCamera.h"


class Game : public MyScene {
private:
	Collider ground;
	Wall wall;
	Fortress fortress;
	EnemyManager enemyManager;
	MyCamera camera;

public:
	Game();
	~Game();
	std::map<int, String>::iterator unchi;

	void update() override;
	void draw() const override;
};

