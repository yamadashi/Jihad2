#pragma once
#include "Scene.h"

class Explain : public MyScene {
private:

public:
	Explain() = default;
	~Explain() = default;

	void update() override;
	void draw() const override;
};