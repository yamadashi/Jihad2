#pragma once
# include "Scene.h"


class Title : public MyScene {
private:
	enum class Select {
		Start,
		Explain,
		End
	} select;

	std::array<String,3> text;

	Font font;
	std::array<Point,2> pos;
	int speed;

	String logo_sub_handler;
	double logo_sub_scale;

public:
	Title();
	~Title() = default;

	void update() override;
	void draw() const override;
};