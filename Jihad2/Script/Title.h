#pragma once
# include "Scene.h"


class Title : public MyScene {
private:
	enum class Select {
		Start,
		Explain,
		End
	} select;

public:
	Title();
	~Title() = default;

	void update() override;
	void draw() const override;
};