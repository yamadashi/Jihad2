#pragma once
# include "Scene.h"
# include "Fortress.h"
# include "Wall.h"
# include "Enemy.h"
# include "MyCamera.h"


class BackGround {
private:
	std::array<Point, 4> cloud_pos, smoke_pos;
	const double aspect_ratio;
	const Point cloud_velocity, smoke_velocity;

public:
	BackGround();
	~BackGround() = default;

	void update();
	void drawCloud() const;
	void drawSmoke() const;
	void drawBG() const;
	void drawGarakuta() const;

};


class Game : public MyScene {
private:
	Collider ground;
	Wall wall;
	Fortress fortress;
	EnemyManager enemyManager;
	MyCamera camera;
	BackGround bg;

	Stopwatch timer;
	Font timeFont;
	bool end;
	int timeLimit; //��

	bool pause;
	enum class PauseChoices { Resume, BackToTile } pauseChoice;
	Font pauseFont1;
	Font pauseFont2;
	std::array<String, 2> pauseChoiceStr;
	HSV pauseChoiceColor;

public:
	Game();
	~Game();

	void update() override;
	void draw() const override;
};