#pragma once
#include "Game.h"

class Tutorial : public MyScene {
private:
	Collider ground;
	Wall wall;
	Fortress fortress;
	EnemyManager enemyManager;
	MyCamera camera;
	BackGround bg;

	Font timeFont;

public:
	Tutorial();
	~Tutorial() = default;

	void update() override;
	void draw() const override;

};