#pragma once
#include "Scene.h"

class Result : public MyScene {
private:

public:
	Result() = default;
	~Result() = default;
	void update() override;
	void draw() const override;

};