#pragma once
#include "Scene.h"

class Result : public MyScene {
private:
	const Font font;

public:
	Result();
	~Result() = default;
	void update() override;
	void draw() const override;

};