#include "Result.h"


Result::Result()
	:font(25)
{}

void Result::update() {
	if (Input::KeyEnter.clicked) changeScene(SceneName::Title);
}

void Result::draw() const {
	font(m_data->cleared ? L"勝った！" : L"負けた！").drawAt(Window::Center());
}